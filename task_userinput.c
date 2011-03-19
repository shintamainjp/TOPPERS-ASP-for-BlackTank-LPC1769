#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

#include "kernel_cfg.h"
#include "task_userinput.h"
#include "task_menu.h"
#include "hid.h"

void task_userinput(intptr_t exinf)
{
    uint8_t s0prev, s1prev, s2prev, s3prev;
    uint8_t v0prev, v1prev, v2prev, v3prev;
    hid_init();
    while(1)
    {
        /*
         * Sense.
         */
        uint8_t s0curr = hid_swread(0);
        uint8_t s1curr = hid_swread(1);
        uint8_t s2curr = hid_swread(2);
        uint8_t s3curr = hid_swread(3);
        uint8_t v0curr = (uint8_t)(hid_volread(0) >> 7);
        uint8_t v1curr = (uint8_t)(hid_volread(1) >> 7);
        uint8_t v2curr = (uint8_t)(hid_volread(2) >> 7);
        uint8_t v3curr = (uint8_t)(hid_volread(3) >> 7);

        /*
         * Diff.
         */
        if (s0prev != s0curr) {
            if (s0curr) {
                USERMSG(SW0, 1);
            } else {
                USERMSG(SW0, 0);
            }
        }
        if (s1prev != s1curr) {
            if (s1curr) {
                USERMSG(SW1, 1);
            } else {
                USERMSG(SW1, 0);
            }
        }
        if (s2prev != s2curr) {
            if (s2curr) {
                USERMSG(SW2, 1);
            } else {
                USERMSG(SW2, 0);
            }
        }
        if (s3prev != s3curr) {
            if (s3curr) {
                USERMSG(SW3, 1);
            } else {
                USERMSG(SW3, 0);
            }
        }
        if (v0prev != v0curr) {
            USERMSG(VOL0, v0curr);
        }
        if (v1prev != v1curr) {
            USERMSG(VOL1, v1curr);
        }
        if (v2prev != v2curr) {
            USERMSG(VOL2, v2curr);
        }
        if (v3prev != v3curr) {
            USERMSG(VOL3, v3curr);
        }

        /*
         * Store.
         */
        s0prev = s0curr;
        s1prev = s1curr;
        s2prev = s2curr;
        s3prev = s3curr;
        v0prev = v0curr;
        v1prev = v1curr;
        v2prev = v2curr;
        v3prev = v3curr;

        tslp_tsk(40);
    }
}

