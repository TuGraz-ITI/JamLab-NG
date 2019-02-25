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

static char doc[] = "JamlabNG pattern viewer.";
static char args_doc[] = "FILENAME";

struct arguments arguments;

static struct argp_option options[] = { 
    { 0 } 
};

struct arguments {
    char* input;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
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
                sscanf(line,"%lu, %u, %u, %u, %u\n", &scenario[scenario_count].ts, &scenario[scenario_count].channel, &scenario[scenario_count].power, &scenario[scenario_count].periode, & scenario[scenario_count].length);
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
        printf("@%lu:\tPower=%d\tChannel=%d\tPeriode=%d\tLength=%d\n",scenario[i].ts, scenario[i].power, scenario[i].channel ,scenario[i].periode, scenario[i].length);
    }
    printf("====================================\n");
}

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


int main(int argc, char **argv)
{
    arguments.input = NULL;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    printf(">>>parsing ...\n");
    parsefile(arguments.input);
    printpattern();
    return 0;
}
