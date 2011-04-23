/**
 * @file task_userinput.c
 * @author Shinichiro Nakamura
 * @brief ユーザインプットタスクの実装(task_userinput)
 * @details
 * ユーザインプットタスクは４系統のユーザ入力の変化を観察するタスク。
 * 変化があればデータキューを介してメニュータスクに通知を行う。
 */

#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

#include "kernel_cfg.h"
#include "task_userinput.h"
#include "task_menu.h"
#include "hid.h"

void task_userinput(intptr_t exinf)
{
    uint16_t initdone = 0;
    uint16_t s0prev = 0, s1prev = 0, s2prev = 0, s3prev = 0;
    uint16_t v0prev = 0, v1prev = 0, v2prev = 0, v3prev = 0;
    hid_init();
    while(1)
    {
        /*
         * Sense.
         */
        uint16_t s0curr = hid_swread(0);
        uint16_t s1curr = hid_swread(1);
        uint16_t s2curr = hid_swread(2);
        uint16_t s3curr = hid_swread(3);
        uint16_t v0curr = (uint16_t)(hid_volread(0) >> 2);
        uint16_t v1curr = (uint16_t)(hid_volread(1) >> 2);
        uint16_t v2curr = (uint16_t)(hid_volread(2) >> 2);
        uint16_t v3curr = (uint16_t)(hid_volread(3) >> 2);

        /*
         * Diff.
         */
        if ((s0prev != s0curr) || (!initdone)) {
            if (s0curr) {
                TSKAPI_MENU_USERMSG(DEVICE_SW0, 1);
            } else {
                TSKAPI_MENU_USERMSG(DEVICE_SW0, 0);
            }
        }
        if ((s1prev != s1curr) || (!initdone)) {
            if (s1curr) {
                TSKAPI_MENU_USERMSG(DEVICE_SW1, 1);
            } else {
                TSKAPI_MENU_USERMSG(DEVICE_SW1, 0);
            }
        }
        if ((s2prev != s2curr) || (!initdone)) {
            if (s2curr) {
                TSKAPI_MENU_USERMSG(DEVICE_SW2, 1);
            } else {
                TSKAPI_MENU_USERMSG(DEVICE_SW2, 0);
            }
        }
        if ((s3prev != s3curr) || (!initdone)) {
            if (s3curr) {
                TSKAPI_MENU_USERMSG(DEVICE_SW3, 1);
            } else {
                TSKAPI_MENU_USERMSG(DEVICE_SW3, 0);
            }
        }
        if ((v0prev != v0curr) || (!initdone)) {
            TSKAPI_MENU_USERMSG(DEVICE_VOL0, v0curr);
        }
        if ((v1prev != v1curr) || (!initdone)) {
            TSKAPI_MENU_USERMSG(DEVICE_VOL1, v1curr);
        }
        if ((v2prev != v2curr) || (!initdone)) {
            TSKAPI_MENU_USERMSG(DEVICE_VOL2, v2curr);
        }
        if ((v3prev != v3curr) || (!initdone)) {
            TSKAPI_MENU_USERMSG(DEVICE_VOL3, v3curr);
        }
        initdone = 1;

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

        tslp_tsk(50);
    }
}

