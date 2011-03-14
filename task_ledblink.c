/**
 * \file task_ledblink.c
 * \brief アプリケーションの本体ファイル。
*/

#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

#include "kernel_cfg.h"
#include "task_ledblink.h"
#include "debled.h"

/**
 * \brief メインタスク
 * \param exinf コンフィギュレータから渡す引数。今回は利用しない
 * \details
 * 100ミリ秒休んでLEDを反転する。これを繰り返す。タスクからは戻らない。
 */
void task_ledblink(intptr_t exinf)
{
    int ledspd = 100;
    int cnt = 0;

    while(1)
    {
        uint_t value;

        while (prcv_dtq(DTQ_LEDSPD, (intptr_t *)&value) == E_OK) {
            if (value > 0) {
                ledspd = value;
                syslog(LOG_NOTICE, "[%d]", ledspd);
            }
        }
        if ((cnt % 2) == 0) {
            debled_set(DEBLED1, 1);
            debled_set(DEBLED2, 1);
            debled_set(DEBLED3, 1);
            debled_set(DEBLED4, 1);
        } else {
            debled_set(DEBLED1, 0);
            debled_set(DEBLED2, 0);
            debled_set(DEBLED3, 0);
            debled_set(DEBLED4, 0);
        }

        tslp_tsk(ledspd);
        cnt++;
    }
}

