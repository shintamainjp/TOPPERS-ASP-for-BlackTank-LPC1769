/**
 * \file task_ledblink.c
 * \brief アプリケーションの本体ファイル。
*/

#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

#include "kernel_cfg.h"
#include "task_ledblink.h"
#include "hardware.h"

/**
 * \brief メインタスク
 * \param exinf コンフィギュレータから渡す引数。今回は利用しない
 * \details
 * 100ミリ秒休んでLEDを反転する。これを繰り返す。タスクからは戻らない。
 */
void task_ledblink(intptr_t exinf)
{
    syslog(LOG_NOTICE, "task_ledblink: Started.");

    int ledspd = 100;
    while(1)
    {
        uint_t value;

        while (prcv_dtq(DTQ_LEDSPD, (intptr_t *)&value) == E_OK) {
            if (value > 0) {
                ledspd = value;
                syslog(LOG_NOTICE, "[%d]", ledspd);
            }
        }

        LPC_GPIO0->FIOPIN ^= ACTLED;
        tslp_tsk(ledspd);
    }
}

