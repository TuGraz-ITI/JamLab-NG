#define BCM2708_PERI_BASE        0x3F000000 
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <time.h>
#include <string.h>

#include <argp.h>

#include <sys/wait.h>

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <sys/socket.h>


static char doc[] = "JamlabNG pattern parser.";
static char args_doc[] = "FILENAME";

int mychannel=14;

struct arguments arguments;

int get_mychannel(){
  printf(">>>channel ...\n");
  struct ifaddrs *ifaddr, *ifa;
  int family, s;
  char host[NI_MAXHOST];
  int ret=14;
  int ip[4];

  if (getifaddrs(&ifaddr) == -1) 
    {
      perror("getifaddrs");
      exit(EXIT_FAILURE);
    }


  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
    {
      if (ifa->ifa_addr == NULL)
        continue;  

      s=getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

      if((strstr(ifa->ifa_name,"enx")!=NULL)&&(ifa->ifa_addr->sa_family==AF_INET))
        {
          if (s != 0)
            {
              printf("getnameinfo() failed: %s\n", gai_strerror(s));
              exit(EXIT_FAILURE);
            }
          printf("Interface:\t<%s>\n",ifa->ifa_name );
          printf("Address:\t<%s>\n", host); 
          sscanf(host,"%d.%d.%d.%d",&ip[0],&ip[1],&ip[2],&ip[3]);
          ret=(ip[3]%14)+1;
          printf("Channel:\t<%d>\n",ret);
        }
    }

  freeifaddrs(ifaddr);
  return ret;
}

static struct argp_option options[] = { 
  { "sync", 's', NULL, 0, "Enable synchronisation to the node reset."},
  { "relative", 'r', NULL, 0, "Relative channel offset."},
  { "loop", 'l', NULL, 0, "Loops pattern at the end."},
  { 0 } 
};

struct arguments {
  bool sync;
  bool loop;
  bool relative;
  char* input;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;
  switch (key) {
  case 's': arguments->sync = true; break;
  case 'l': arguments->loop = true; break;
  case 'r': arguments->relative = true; break;
  case ARGP_KEY_NO_ARGS:
    argp_usage (state);
  case ARGP_KEY_ARG: 
    //return 0;
    arguments->input = arg;
    //arguments->strings = &state->argv[state->next];
    state->next = state->argc;
    break;
  default: return ARGP_ERR_UNKNOWN;
  }   
  return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

typedef struct {
  uint64_t ts;
  uint8_t channel;
  uint16_t power;
  uint16_t periode;
  uint16_t length;
} jampattern_t;

jampattern_t* scenario;
int scenario_count;

int parsefile(char* filename){
  FILE *file = fopen ( filename, "r" );
  scenario=malloc(sizeof(jampattern_t)); //alloc one line
  scenario_count=0;
  if ( file != NULL )
    {
      char line [ 1024 ]; /* or other suitable maximum line size */
      while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
        {
          if(strlen(line) > 1) {
            sscanf(line,"%llu, %d, %d, %d, %d\n", &scenario[scenario_count].ts, &scenario[scenario_count].channel, &scenario[scenario_count].power, &scenario[scenario_count].periode, & scenario[scenario_count].length);
            scenario_count++;
            scenario=realloc(scenario,(1+scenario_count)*sizeof(jampattern_t));
          }
        }
      fclose ( file );
    }
  else
    {
      printf("File does not exit!\n");
      exit(-1);
    }
  return 0;
}

void printpattern(){
  printf("Jamming patterns are:\n");
  for(int i=0;i<scenario_count;i++){
    printf("@%llu:\tPower=%d\tChannel=%d\tPeriode=%d\tLength=%d\n",scenario[i].ts, scenario[i].power, scenario[i].channel ,scenario[i].periode, scenario[i].length);
  }
  printf("====================================\n");
}

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
void *gpio_map;

// I/O access
volatile unsigned *gpio;

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38) // Pull up/pull down clock

void setup_io();

#define NANO 1000000000L

// buf needs to store 30 characters
int timespec2str(char *buf, uint len, struct timespec *ts) {
  int ret;
  struct tm t;

  tzset();
  if (localtime_r(&(ts->tv_sec), &t) == NULL)
    return 1;

  ret = strftime(buf, len, "%F %T", &t);
  if (ret == 0)
    return 2;
  len -= ret - 1;

  ret = snprintf(&buf[strlen(buf)], len, ".%09ld", ts->tv_nsec);
  if (ret >= len)
    return 3;

  return 0;
}

void exec_nexutil(char** argv){
  pid_t pid=fork();
  if(pid==0){
    int fd=open( "/dev/null", O_WRONLY ); 
    dup2(fd,1);
    dup2(fd,2);
    execv("/usr/bin/nexutil",argv);
    exit(-1);
  } else {
    waitpid(pid,0,0);
  }
  return; 
}

char *itoa(long i, char* s, int dummy_radix) {
  sprintf(s, "%ld", i);
  return s;
}

void setup_jamming(){
  // zhitao: set rate = N * 500 kbps (e.g. 108 for 54 Mbps)
  char* argv[]={"nexutil", "-s0x713", "-i", "-v", "12",NULL};
  exec_nexutil(argv);
}

void update_jamming(jampattern_t p){
  char buf[10];

  if(p.power==0){
    char* argv2[]={"nexutil","-s0x700",NULL};
    exec_nexutil(argv2);
    return;
  }
  itoa(p.power,buf,10);	    
  char* argv2[]={"nexutil","-s0x704", "-i", "-v", buf,NULL};
  exec_nexutil(argv2);

  itoa(p.periode,buf,10);	    
  char* argv1[]={"nexutil","-s0x706", "-i", "-v", buf,NULL};
  exec_nexutil(argv1);

  itoa(p.length,buf,10);	    
  char* argv3[]={"nexutil","-s0x707", "-i", "-v", buf,NULL};
  exec_nexutil(argv3);

  int channel=p.channel;
  if(arguments.relative || channel==0){
    channel+=mychannel;
    if(channel>14)
      channel=(channel%14)+1;
  }
  itoa(channel,buf,10); 
  char* argv4[]={"nexutil","-s0x702", "-i", "-v", buf,NULL};
  // zhitao: 0x712 setting channel to 40 MHz does not have effect
  /* char* argv4[]={"nexutil","-s0x712", "-i", "-v", buf,NULL}; */
  exec_nexutil(argv4);

  char* argv5[]={"nexutil","-s0x701", "-i", "-v", buf,NULL};
  exec_nexutil(argv5);

}

void stop_jamming(){
  char* argv[]={"nexutil","-s0x700",NULL};
  exec_nexutil(argv);
}

bool terminate=false;
void term(int signum){
  stop_jamming();
  printf("exiting gracefully\n");
  terminate=true;
  exit(-1);
}

int main(int argc, char **argv)
{
  mychannel=get_mychannel();
  arguments.input = NULL;
  arguments.sync = false;
  arguments.loop = false;
  arguments.relative = false;

  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  printf(">>>arguments ...\n");

  if(arguments.relative)
    printf("relative mode enabled\n");
  if(arguments.loop)
    printf("loop mode enabled\n");


  // Set up gpi pointer for direct register access
  setup_io();
    
  struct sigaction action;
  memset(&action, 0, sizeof(struct sigaction));
  action.sa_handler = term;
  sigaction(SIGTERM, &action, NULL);
  sigaction(SIGHUP, &action, NULL);
  sigaction(SIGINT, &action, NULL);

  //int clk_id = CLOCK_REALTIME;
  int clk_id = CLOCK_MONOTONIC;
  const uint TIME_FMT = strlen("2012-12-31 12:59:59.123456789") + 1;
  char timestr[TIME_FMT];

  //    /* Power=21, Reset=20, EN1=16, EN2=12 */

  struct timespec ts,start;
  //clock_getres(clk_id, &res);

  printf(">>>parsing ...\n");
  parsefile(arguments.input);
  printpattern();

  int count=0;
  setup_jamming();


  if (arguments.sync){
    printf(">>>waiting for reset ...\n");
    while(GET_GPIO(20)!=0){usleep(100);}
    while(GET_GPIO(20)==0){usleep(100);}
  }

  printf(">>>starting ...\n");
  clock_gettime(clk_id, &start);
  while(!terminate)
    {
      ts.tv_sec=start.tv_sec;
      ts.tv_nsec=start.tv_nsec;

      jampattern_t scen=scenario[count++];
      //clock_gettime(clk_id,&ts);
      uint64_t interval=scen.ts;
      while(interval > NANO) {
        ts.tv_sec++;
        interval -=NANO;
      }
      ts.tv_nsec += interval;
      while (ts.tv_nsec >= NANO) {
        ts.tv_nsec -= NANO;
        ts.tv_sec++;
      }
      clock_nanosleep(clk_id, TIMER_ABSTIME, &ts, NULL);

      update_jamming(scen);

      if(count==scenario_count){
        //break;
        if(arguments.loop && scenario_count!=1){
          printf("pattern finished, restart from the begining...\n");
          count=0;
          clock_gettime(clk_id, &start);
        }
        else{
          printf("pattern finished, looping forever...\n");
          while(!terminate){sleep(1);}
        }
      }
    }

  stop_jamming();

  return 0;

} // main


//
// Set up a memory regions to access GPIO
//
void setup_io()
{
  /* open /dev/mem */
  if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
    printf("can't open /dev/mem \n");
    exit(-1);
  }

  /* mmap GPIO */
  gpio_map = mmap(
                  NULL,             //Any adddress in our space will do
                  BLOCK_SIZE,       //Map length
                  PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
                  MAP_SHARED,       //Shared with other processes
                  mem_fd,           //File to map
                  GPIO_BASE         //Offset to GPIO peripheral
                  );

  close(mem_fd); //No need to keep mem_fd open after mmap

  if (gpio_map == MAP_FAILED) {
    printf("mmap error %d\n", (int)gpio_map);//errno also set!
    exit(-1);
  }

  // Always use volatile pointer!
  gpio = (volatile unsigned *)gpio_map;


} // setup_io
