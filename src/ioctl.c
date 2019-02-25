
/***************************************************************************
 *                                                                         *
 *          ###########   ###########   ##########    ##########           *
 *         ############  ############  ############  ############          *
 *         ##            ##            ##   ##   ##  ##        ##          *
 *         ##            ##            ##   ##   ##  ##        ##          *
 *         ###########   ####  ######  ##   ##   ##  ##    ######          *
 *          ###########  ####  #       ##   ##   ##  ##    #    #          *
 *                   ##  ##    ######  ##   ##   ##  ##    #    #          *
 *                   ##  ##    #       ##   ##   ##  ##    #    #          *
 *         ############  ##### ######  ##   ##   ##  ##### ######          *
 *         ###########    ###########  ##   ##   ##   ##########           *
 *                                                                         *
 *            S E C U R E   M O B I L E   N E T W O R K I N G              *
 *                                                                         *
 * Copyright (c) 2018 Matthias Schulz                                      *
 *                                                                         *
 * Permission is hereby granted, free of charge, to any person obtaining a *
 * copy of this software and associated documentation files (the           *
 * "Software"), to deal in the Software without restriction, including     *
 * without limitation the rights to use, copy, modify, merge, publish,     *
 * distribute, sublicense, and/or sell copies of the Software, and to      *
 * permit persons to whom the Software is furnished to do so, subject to   *
 * the following conditions:                                               *
 *                                                                         *
 * 1. The above copyright notice and this permission notice shall be       *
 *    include in all copies or substantial portions of the Software.       *
 *                                                                         *
 * 2. Any use of the Software which results in an academic publication or  *
 *    other publication which includes a bibliography must include         *
 *    citations to the nexmon project a) and the paper cited under b) or   *
 *    the thesis cited under c):                                           *
 *                                                                         *
 *    a) "Matthias Schulz, Daniel Wegemer and Matthias Hollick. Nexmon:    *
 *        The C-based Firmware Patching Framework. https://nexmon.org"     *
 *                                                                         *
 *    b) "Matthias Schulz, Jakob Link, Francesco Gringoli, and Matthias    *
 *        Hollick. Shadow Wi-Fi: Teaching Smart- phones to Transmit Raw    *
 *        Signals and to Extract Channel State Information to Implement    *
 *        Practical Covert Channels over Wi-Fi. Accepted to appear in      *
 *        Proceedings of the 16th ACM International Conference on Mobile   *
 *        Systems, Applications, and Services (MobiSys 2018), June 2018."  *
 *                                                                         *
 *    c) "Matthias Schulz. Teaching Your Wireless Card New Tricks:         *
 *        Smartphone Performance and Security Enhancements through Wi-Fi   *
 *        Firmware Modifications. Dr.-Ing. thesis, Technische Universität  *
 *        Darmstadt, Germany, February 2018."                              *
 *                                                                         *
 * 3. The Software is not used by, in cooperation with, or on behalf of    *
 *    any armed forces, intelligence agencies, reconnaissance agencies,    *
 *    defense agencies, offense agencies or any supplier, contractor, or   *
 *    research associated.                                                 *
 *                                                                         *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF              *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY    *
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,    *
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE       *
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  *
 *                                                                         *
 **************************************************************************/

#pragma NEXMON targetregion "patch"

#include <firmware_version.h>   // definition of firmware version macros
#include <debug.h>              // contains macros to access the debug hardware
#include <wrapper.h>            // wrapper definitions for functions that already exist in the firmware
#include <structs.h>            // structures that are used by the code in the firmware
#include <helper.h>             // useful helper functions
#include <patcher.h>            // macros used to craete patches such as BLPatch, BPatch, ...
#include <rates.h>              // rates used to build the ratespec for frame injection
#include <nexioctls.h>          // ioctls added in the nexmon patch
#include <version.h>            // version information
#include <argprintf.h>          // allows to execute argprintf to print into the arg buffer
#include <channels.h>

#include <sendframe.h>

#include <local_wrapper.h>
#include <jamlab.h>

#define PHYREF_SamplePlayStartPtr           u.d11acregs.SamplePlayStartPtr
#define PHYREF_SamplePlayStopPtr            u.d11acregs.SamplePlayStopPtr
#define PHYREF_SampleCollectPlayCtrl        u.d11acregs.SampleCollectPlayCtrl

#define wreg32(r, v)        (*(volatile uint32*)(r) = (uint32)(v))
#define rreg32(r)       (*(volatile uint32*)(r))
#define wreg16(r, v)        (*(volatile uint16*)(r) = (uint16)(v))
#define rreg16(r)       (*(volatile uint16*)(r))
#define wreg8(r, v)     (*(volatile uint8*)(r) = (uint8)(v))
#define rreg8(r)        (*(volatile uint8*)(r))

#define BCM_REFERENCE(data) ((void)(data))

#define W_REG(osh, r, v) do { \
    BCM_REFERENCE(osh); \
    switch (sizeof(*(r))) { \
    case sizeof(uint8): wreg8((void *)(r), (v)); break; \
    case sizeof(uint16):    wreg16((void *)(r), (v)); break; \
    case sizeof(uint32):    wreg32((void *)(r), (v)); break; \
    } \
} while (0)

#define R_REG(osh, r) ({ \
    __typeof(*(r)) __osl_v; \
    BCM_REFERENCE(osh); \
    switch (sizeof(*(r))) { \
    case sizeof(uint8): __osl_v = rreg8((void *)(r)); break; \
    case sizeof(uint16):    __osl_v = rreg16((void *)(r)); break; \
    case sizeof(uint32):    __osl_v = rreg32((void *)(r)); break; \
    } \
    __osl_v; \
})

#define PHY_CORE_MAX 8
//#define PHY_CORE_MAX 1
#define PHY_NOISE_SAMPLE_LOG_NUM_NPHY 10
#define ACPHY_ClassifierCtrl_classifierSel_MASK 0x7

void reset_deaf(struct wlc_info *wlc)
{
  int mpc = 0; //turn off power management
  wlc_iovar_op(wlc, "mpc", 0, 0, &mpc, sizeof(mpc), 1, 0);

  wlc_lcn40phy_deaf_mode(wlc->hw->band->pi, deaf);
  //wlc_bmac_suspend_mac_and_wait(wlc->hw);
  //wlc_phy_stay_in_carriersearch_acphy(wlc->band->pi, deaf);
  //wlc_bmac_enable_mac(wlc->hw);
}

bool wlc_send_data(struct wlc_info *wlc)
{
  //printf("sending\n");
  sk_buff *p = pkt_buf_get_skb(wlc->osh, packet_len + 202);
  if(p != 0)
  {
    //reset_deaf(wlc); //make sure deaf mode is set correctly
    wlc_lcn40phy_deaf_mode(wlc->hw->band->pi, deaf);
    struct txdata *packet_skb;
    packet_skb = (struct txdata *) skb_pull(p, 202);
    memcpy(packet_skb, &data, packet_len);
    sendframe(wlc, p, 1, transmission_speed);
    return 1;
  }
  else
  {
//    printf("error allocating packet\n");
    return 0;
  }
}

void _set_chanspec(struct wlc_info *wlc, unsigned short chanspec)
{
  unsigned int local_chanspec = chanspec;
  wlc_iovar_op(wlc, "chanspec", 0, 0, &local_chanspec, 4, 1, 0);
}

unsigned int _get_chanspec(struct wlc_info *wlc)
{
  unsigned int chanspec = 0;
  wlc_iovar_op(wlc, "chanspec", 0, 0, &chanspec, 4, 0, 0);
  return chanspec;
}

void
set_txpwr_qdbm(struct wlc_info *wlc, signed int qdbm)
{
  wlc_iovar_op(wlc, "qtxpower", 0, 0, &qdbm, 4, 1, 0);
}

void
set_txpwr_mw(struct wlc_info *wlc, int mw)
{
  set_txpwr_qdbm(wlc, bcm_mw_to_qdbm(mw));
}

void 
task_timer_cb(struct hndrte_timer *t)
{
    //printf("tick.................................\n");
    if(task_stop==0)
    {
      struct wlc_info *wlc = (struct wlc_info *) t->data;
      sendframe_with_hwtimer(wlc, &data, packet_len, deaf, 1, transmission_speed, 0, packet_count-1, txdelay);
      hndrte_add_timer(task_timer, task_interval, 0);
    }
}

void 
timer_cb(struct hndrte_timer *t)
{
    //printf("tick.................................\n");
    if(stop==0)
    {
      struct wlc_info *wlc = (struct wlc_info *) t->data;
      wlc_send_data(wlc);
      hndrte_add_timer(timer, interval, 0);
    }
}

void 
hwtimer_cb(void *wlc)
{
    //printf("tick.................................\n");
    if(hwstop==0)
    {
      wlc_send_data((struct wlc_info*) wlc);
      wlc_hwtimer_add_timeout(hwtimer, hwinterval, hwtimer_cb, wlc);
    }
}



int 
wlc_ioctl_hook(struct wlc_info *wlc, int cmd, char *arg, int len, void *wlc_if)
{
    int ret = IOCTL_ERROR;
    argprintf_init(arg, len);

    switch(cmd) {
    case 0x700:
      {
        if(timer==0)
        {         
          printf("timer already stopped...\n");
          ret = IOCTL_SUCCESS;
          break;
        }
        printf("stopping timer...\n");
        stop=1;
        hndrte_del_timer(timer);
        hndrte_free_timer(timer);
        timer=0;
        int mpc = 1; //turn off power management
        wlc_iovar_op(wlc, "mpc", 0, 0, &mpc, sizeof(mpc), 1, 0);
        wlc_bmac_suspend_mac_and_wait(wlc->hw);

        ret = IOCTL_SUCCESS;
      }
      break;

    case 0x701:
      {
        if(timer!=0)
        {         
          printf("timer already started...\n");
          ret = IOCTL_SUCCESS;
          break;
        }
        printf("starting timer...\n");
        stop=0;

        int mpc = 0; //turn off power management
        wlc_iovar_op(wlc, "mpc", 0, 0, &mpc, sizeof(mpc), 1, 0);
        _set_chanspec(wlc, CH20MHZ_CHSPEC(channel));
        reset_deaf(wlc);

        timer = schedule_work(wlc_ioctl_hook, wlc, timer_cb, interval+1000, 0);

        //wlc_send_data(wlc);
        ret = IOCTL_SUCCESS;
      }
      break;



    case 0x770:
      {
        if(task_timer==0)
        {         
          printf("task timer already stopped...\n");
          ret = IOCTL_SUCCESS;
          break;
        }
        printf("stopping task timer...\n");
        task_stop=1;
        hndrte_del_timer(task_timer);
        hndrte_free_timer(task_timer);
        task_timer=0;
        int mpc = 1; //turn off power management
        wlc_iovar_op(wlc, "mpc", 0, 0, &mpc, sizeof(mpc), 1, 0);
        wlc_bmac_suspend_mac_and_wait(wlc->hw);

        ret = IOCTL_SUCCESS;
      }
      break;

    case 0x771:
      {
        if(task_timer!=0)
        {         
          printf("task timer already started...\n");
          ret = IOCTL_SUCCESS;
          break;
        }
        printf("starting task timer...\n");
        task_stop=0;

        int mpc = 0; //turn off power management
        wlc_iovar_op(wlc, "mpc", 0, 0, &mpc, sizeof(mpc), 1, 0);
        _set_chanspec(wlc, CH20MHZ_CHSPEC(channel));
        reset_deaf(wlc);

        task_timer = schedule_work(wlc_ioctl_hook, wlc, task_timer_cb, task_interval+1000, 0);

        //wlc_send_data(wlc);
        ret = IOCTL_SUCCESS;
      }
      break;

    case 0x702:
      {
        unsigned char tmp_channel=channel;
        if (arg != 0) {
          tmp_channel = *((unsigned int*)arg);
        } else {
            ret = IOCTL_SUCCESS;
            break;
        }
        if (tmp_channel > 14 || tmp_channel == 0) {
          ret = IOCTL_SUCCESS;
          break;
        }
        channel=tmp_channel;
        printf("setting channel %d (at 20MHz)...\n",channel);
        _set_chanspec(wlc, CH20MHZ_CHSPEC(channel));
        reset_deaf(wlc);
        ret = IOCTL_SUCCESS;
      }
      break;

    case 0x713:
      {
        unsigned int tmp_speed=transmission_speed;
        printf("setting transmission speed...\n",channel);
        if (arg != 0) {
          tmp_speed = *((unsigned int*)arg);
        } else {
            ret = IOCTL_SUCCESS;
            break;
        }
        if (tmp_speed > 20000 ) {
          ret = IOCTL_SUCCESS;
          break;
        }
        transmission_speed=tmp_speed;
        printf("new speed is %d...\n",transmission_speed);
        ret = IOCTL_SUCCESS;
      }
      break;

    case 0x712:
      {
        unsigned char tmp_channel=channel;
        if (arg != 0) {
          tmp_channel = *((unsigned int*)arg);
        } else {
            ret = IOCTL_SUCCESS;
            break;
        }
        if (tmp_channel > 14 || tmp_channel == 0) {
          ret = IOCTL_SUCCESS;
          break;
        }
        channel=tmp_channel;
        printf("setting channel %d (at 40MHz)...\n",channel);
        _set_chanspec(wlc, CH40MHZ_CHSPEC(channel,WL_CHANSPEC_CTL_SB_LOWER));
        reset_deaf(wlc);
        ret = IOCTL_SUCCESS;
      }
      break;

    case 0x722:
      {
        unsigned char tmp_channel=channel;
        if (arg != 0) {
          tmp_channel = *((unsigned int*)arg);
        } else {
            ret = IOCTL_SUCCESS;
            break;
        }
        if (tmp_channel > 14 || tmp_channel == 0) {
          ret = IOCTL_SUCCESS;
          break;
        }
        channel=tmp_channel;
        printf("setting channel %d (at 80MHz)...\n",channel);
        _set_chanspec(wlc, CH80MHZ_CHSPEC(channel,WL_CHANSPEC_CTL_SB_LOWER));
        reset_deaf(wlc);
        ret = IOCTL_SUCCESS;
      }
      break;

    case 0x703:
      {
        unsigned char tmp_deaf=deaf;
        if (arg != 0) {
          tmp_deaf = *((unsigned int*)arg);
        } else {
            ret = IOCTL_SUCCESS;
            break;
        }
        if (!(tmp_deaf == 1 || tmp_deaf == 0)) {
          ret = IOCTL_SUCCESS;
          break;
        }
        
        if(tmp_deaf==0)
        {
          printf("disabling deaf mode...\n");
          int mpc = 0; //turn off power management
          wlc_iovar_op(wlc, "mpc", 0, 0, &mpc, sizeof(mpc), 1, 0);
          deaf=0;
          reset_deaf(wlc);
          ret = IOCTL_SUCCESS;
        }
        else if(tmp_deaf==1)
        {
          printf("enabling deaf mode...\n");
          int mpc = 0; //turn off power management
          wlc_iovar_op(wlc, "mpc", 0, 0, &mpc, sizeof(mpc), 1, 0);
          deaf=1;
          reset_deaf(wlc);
          ret = IOCTL_SUCCESS;

        }
      }
      break;

    case 0x704:
      {
        unsigned short tmp_pwr=0;
        if (arg != 0)
        {
          tmp_pwr=*((unsigned short*)arg);
        }
        printf("setting tx power (mW)...\n");
        if (tmp_pwr<1500 && tmp_pwr>=0) 
        {
          pwr_db = bcm_mw_to_qdbm(tmp_pwr);
          printf("new power: %dmW / %ddB\n",tmp_pwr,pwr_db);
          set_txpwr_qdbm(wlc,pwr_db);
          reset_deaf(wlc);
        }
        else
        {
            printf("new power out of range (0-1500)mW...\n");
        }
        ret = IOCTL_SUCCESS;
      }
      break;

    case 0x705:
      {
        unsigned short tmp_pwr=0;
        if (arg != 0)
        {
          tmp_pwr=*((unsigned short*)arg);
        }
        printf("setting tx power (dB)...\n");
        if (tmp_pwr<127 && tmp_pwr>=0) 
        {
          pwr_db = tmp_pwr;
          printf("new power: %ddB\n",pwr_db);
          set_txpwr_qdbm(wlc,pwr_db);
          reset_deaf(wlc);
        }
        else
        {
            printf("new power out of range (0-127)db...\n");
        }
        ret = IOCTL_SUCCESS;
      }
      break;

    case 0x706:
      {
        unsigned short tmp_interval=interval;
        if (arg != 0)
        {
          tmp_interval=*((unsigned short*)arg);
        }
        printf("setting os timer interval in ms...\n");
        if (tmp_interval<10000 && tmp_interval>=1) 
        {
          interval = tmp_interval;
          printf("new interval: %dms\n",interval);
        }
        else
        {
            printf("new interval out of range (1-10000)ms...\n");
        }
        reset_deaf(wlc);
        ret = IOCTL_SUCCESS;
      }
      break;

    case 0x707:
      {
        unsigned short tmp_length=packet_len;
        if (arg != 0)
        {
          tmp_length=*((unsigned short*)arg);
        }
        printf("setting packet length in bytes...\n");
        if (tmp_length<=MAX_JAM_PACK && tmp_length>=MIN_JAM_PACK) 
        {
          packet_len = tmp_length;
          printf("new packet length: %dbytes\n",packet_len);
        }
        else
        {
            printf("new packet length out of range (%d-%d) bytes...\n",MIN_JAM_PACK,MAX_JAM_PACK);
        }
        reset_deaf(wlc);
        ret = IOCTL_SUCCESS;
      }
      break;

    case 0x777:
      {
        unsigned short tmp_interval=task_interval;
        if (arg != 0)
        {
          tmp_interval=*((unsigned short*)arg);
        }
        printf("setting task timer interval in ms...\n");
        if (tmp_interval<100000 && tmp_interval>=1) 
        {
          task_interval = tmp_interval;
          printf("new task interval: %dms\n",task_interval);
        }
        else
        {
            printf("new interval out of range (1-100000)ms...\n");
        }
        reset_deaf(wlc);
        ret = IOCTL_SUCCESS;
      }
      break;

    case 0x778:
      {
        unsigned short tmp_count=packet_count;
        if (arg != 0)
        {
          tmp_count=*((unsigned short*)arg);
        }
        printf("setting packet repeat count...\n");
        if (tmp_count<=MAX_JAM_COUNT && tmp_count>=MIN_JAM_COUNT) 
        {
          packet_count = tmp_count;
          printf("new packet count: %d\n",packet_count);
        }
        else
        {
            printf("new packet count out of range (%d-%d)...\n",MIN_JAM_COUNT,MAX_JAM_COUNT);
        }
        reset_deaf(wlc);
        ret = IOCTL_SUCCESS;
      }
      break;

    case 0x779:
      {
        unsigned short tmp_interval=txdelay;
        if (arg != 0)
        {
          tmp_interval=*((unsigned short*)arg);
        }
        printf("setting tx delay timer interval in us...\n");
        if (tmp_interval<=5000 && tmp_interval>=0) 
        {
          txdelay = tmp_interval;
          printf("new tx delay interval: %dus\n",txdelay);
        }
        else
        {
            printf("new interval out of range (0-5000)us...\n");
        }
        reset_deaf(wlc);
        ret = IOCTL_SUCCESS;
      }
      break;

    case 0x760:
      {
        if(hwtimer==0)
        {         
          printf("hwtimer already stopped...\n");
          ret = IOCTL_SUCCESS;
          break;
        }
        printf("stopping hwtimer...\n");
        hwstop=1;
        wlc_hwtimer_del_timeout(hwtimer);
        hwtimer=0;
        int mpc = 1; //turn off power management
        wlc_iovar_op(wlc, "mpc", 0, 0, &mpc, sizeof(mpc), 1, 0);
        wlc_bmac_suspend_mac_and_wait(wlc->hw);

        ret = IOCTL_SUCCESS;
      }
      break;

    case 0x761:
      {
        if(hwtimer!=0)
        {         
          printf("hwtimer already started...\n");
          ret = IOCTL_SUCCESS;
          break;
        }
        printf("starting hwtimer...\n");
        hwstop=0;

        int mpc = 0; //turn off power management
        wlc_iovar_op(wlc, "mpc", 0, 0, &mpc, sizeof(mpc), 1, 0);
        _set_chanspec(wlc, CH20MHZ_CHSPEC(channel));
        reset_deaf(wlc);

        //timer = schedule_work(wlc_ioctl_hook, wlc, timer_cb, interval, 0);
        hwtimer = wlc_hwtimer_alloc_timeout(wlc->hrti);
        wlc_hwtimer_add_timeout(hwtimer,hwinterval,hwtimer_cb,wlc);

        wlc_send_data(wlc);
        ret = IOCTL_SUCCESS;
      }
      break;

    case 0x766:
      {
        unsigned short tmp_hwinterval=hwinterval;
        if (arg != 0)
        {
          tmp_hwinterval=*((unsigned int*)arg);
        }
        printf("setting hw timer interval in us...\n");
        if (tmp_hwinterval<1000000 && tmp_hwinterval>=1) 
        {
          hwinterval = tmp_hwinterval;
          printf("new hw interval: %dus\n",hwinterval);
        }
        else
        {
            printf("new hw interval out of range (1-1000000)us...\n");
        }
        reset_deaf(wlc);
        ret = IOCTL_SUCCESS;
      }
      break;

    case 0x740:
      {
        unsigned short tmp_seed=0xDEADu;
        if (arg != 0)
        {
          printf("setting random seed...\n");
          tmp_seed=*((unsigned short*)arg);
          prng_seed(tmp_seed);
          printf("new random seed is %d\n",tmp_seed);
        }
        ret = IOCTL_SUCCESS;
      }
      break;

    case 0x741:
      {
        printf("magic 8ball say %d\n",prng_rand());
        ret = IOCTL_SUCCESS;
      }
      break;


    default:
        {
            ret = wlc_ioctl(wlc, cmd, arg, len, wlc_if);
        }
    }
    return ret;
}

__attribute__((at(0x4305c, "", CHIP_VER_BCM43430a1, FW_VER_7_45_41_46)))
GenericPatch4(wlc_ioctl_hook, wlc_ioctl_hook + 1);
