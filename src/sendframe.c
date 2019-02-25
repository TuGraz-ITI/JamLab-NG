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
 * This file is part of NexMon.                                            *
 *                                                                         *
 * Copyright (c) 2016 NexMon Team                                          *
 *                                                                         *
 * NexMon is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * NexMon is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with NexMon. If not, see <http://www.gnu.org/licenses/>.          *
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
#include <capabilities.h>       // capabilities included in a nexmon patch

#include <local_wrapper.h>


void
sendframe(struct wlc_info *wlc, struct sk_buff *p, unsigned int fifo, unsigned int rate)
{
    int short_preamble = 0;
    struct wlc_txh_info txh = {0};

    if(wlc->band->hwrs_scb) {
        if (wlc->hw->up) {
            wlc_d11hdrs(wlc, p, wlc->band->hwrs_scb, short_preamble, 0, 1, 1, 0, 0, rate);
            p->scb = wlc->band->hwrs_scb;
            wlc_get_txh_info(wlc, p, &txh);
            wlc_txfifo(wlc, 1, p, &txh, 1, 1);
        } else {
            printf("ERR: wlc down\n");
        }
    } else {
        printf("ERR: no scb found, discarding packet!\n");
        pkt_buf_free_skb(wlc->osh, p, 0);
    }
}

struct tx_task {
    struct wlc_info *wlc;
    struct txdata* data;
    unsigned int len;
    unsigned int deaf;
    unsigned int fifo;
    unsigned int rate;
    int txrepetitions;
    int txperiodicity;
    void *timer;
};

void
print_task(struct tx_task *task)
{
    printf("wlc_info %d\n",(unsigned int) task->wlc);
    printf("data %d\n",(unsigned int) task->data);
    printf("len %d\n", task->len);
    printf("fifo %d\n", task->fifo);
    printf("rate %d\n", task->rate);
    printf("txrepetitions %d\n", task->txrepetitions);
    printf("txperiodicity %d\n", task->txperiodicity);
    printf("timer %d\n", task->timer);
}

static int
sendframe_adhoc(struct tx_task *task)
{
    sk_buff *p = pkt_buf_get_skb(task->wlc->osh, task->len + 202);
    if(p != 0)
    {
        //reset_deaf(wlc); //make sure deaf mode is set correctly
        wlc_lcn40phy_deaf_mode(task->wlc->hw->band->pi, task->deaf);
        struct txdata *packet_skb;
        packet_skb = (struct txdata *) skb_pull(p, 202);
        memcpy(packet_skb, task->data, task->len);
        sendframe(task->wlc, p, task->fifo, task->rate);
        return 0;
    }
    return 1;
}

static void
sendframe_task(struct tx_task *task)
{
    if(!sendframe_adhoc(task)){
        if (task->txrepetitions > 0) {
            task->txrepetitions--;
        }
    } else{
        printf("Packet allocation failed\n");
    }
}

static void
//sendframe_timer_handler(struct hndrte_timer *t)
sendframe_timer_handler(void *t)
{
    //struct tx_task *task = (struct tx_task *) t->data;
    struct tx_task *task = (struct tx_task*) t;

    if (task->txrepetitions == 0) {
        // there must have been a mistake, just delete the frame task and timer
        goto free_timer_and_task;
    } else if (task->txrepetitions == 1) {
        sendframe_adhoc(task);
free_timer_and_task:
        if(task->timer!=0){ 
            wlc_hwtimer_free_timeout(task->timer);
            task->timer=0;
        }
        free(task);
    } else {
        wlc_hwtimer_add_timeout(task->timer,task->txperiodicity,sendframe_timer_handler,task);
        sendframe_task(task);
    }
}

static void
sendframe_repeatedly(struct tx_task *task)
{
    if(task->txperiodicity==0){
        while(task->txrepetitions>0){
            sendframe_task(task);
        }
        free(task);
        return;
    }

    if(task->timer!=0){
        printf("ERR: timer already exists!\n");
        return;
    }

    void * t;

    sendframe_task(task);
    if (task->txrepetitions == 0)
        return;

    t = wlc_hwtimer_alloc_timeout(task->wlc->hrti);

    if (!t) {
        printf("ERR: could not create timer\n");
        free(task);
        return;
    }

    task->timer=t;

    if(!wlc_hwtimer_add_timeout(t,task->txperiodicity,sendframe_timer_handler,task)) {
        wlc_hwtimer_free_timeout(t);
        free(task);
        printf("ERR: could not add timer\n");
    }
}

/**
 *  Is scheduled to transmit a frame after a delay
 */
static void
sendframe_task_handler(struct hndrte_timer *t)
{
    struct tx_task *task = (struct tx_task *) t->data;
    if (task->txrepetitions != 0) {
        sendframe_repeatedly(task);
    } else {
        sendframe_adhoc(task);
        free(task);
    }
}

void
sendframe_with_hwtimer(struct wlc_info *wlc, struct txdata *data, unsigned int len, unsigned int deaf, unsigned int fifo, unsigned int rate, int txdelay, int txrepetitions, int txperiodicity)
{
    struct tx_task *task = 0;

    task = (struct tx_task *) malloc(sizeof(struct tx_task), 0);
    memset(task, 0, sizeof(struct tx_task)); // will be freed after finishing the task
    task->wlc = wlc;
    task->data = data;
    task->len = len;
    task->deaf = deaf;
    task->fifo = fifo;
    task->rate = rate;
    task->txrepetitions = txrepetitions;
    task->txperiodicity = txperiodicity;
    task->timer=0;

    if (txdelay > 0) {
        hndrte_schedule_work(sendframe_with_hwtimer, task, sendframe_task_handler, txdelay);
    } else if (txrepetitions != 0) {
        sendframe_repeatedly(task);
    } else {
        sendframe_adhoc(task);
        free(task);
    }
}
