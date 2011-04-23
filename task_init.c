/**
 * @file task_init.c
 * @author Shinichiro Nakamura
 * @brief 初期化タスクの実装(task_init)
 * @details
 * よくあるスプラッシュ画面を表示するタスク。
 * このタスクは自身の処理が完了後に、本当に必要なタスクを起動する。
 * 本当に必要なタスクを起動した後は、システムの動作をユーザに通知する
 * だけのタスクとして動作する。
 */

#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>

#include "task_init.h"
#include "kernel_cfg.h"
#include "task_display.h"
#include "config.h"

#define TASK_START(TSKID,TSKNAME) act_tsk(TSKID); tslp_tsk(100); syslog(LOG_NOTICE, "[%s]", TSKNAME)

void task_init(intptr_t exinf)
{
    TASK_START(TASK_AUDIO, "audio");
    TASK_START(TASK_DISPLAY, "display");
    TASK_START(TASK_LED, "led");
    TASK_START(TASK_MENU, "menu");
    TASK_START(TASK_USERINPUT, "userinput");
    TASK_START(TASK_NTSHELL, "ntshell");

    if (TASK_AUDIO_ENABLED()) {
    } else {
        syslog(LOG_NOTICE, "task_audio disabled in this build.");
    }

    int cnt = 0;
    while (1) {
        /*
         * 動作を示すマーキング.
         */
        TSKAPI_DISPLAY_FILLBOX(90, 0, 95, 6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
        switch (cnt % 4) {
            case 0:
                TSKAPI_DISPLAY_TEXT(90, 0, 0xFF, 0xFF, 0xFF, "-");
                break;
            case 1:
                TSKAPI_DISPLAY_TEXT(90, 0, 0xFF, 0xFF, 0xFF, "\\");
                break;
            case 2:
                TSKAPI_DISPLAY_TEXT(90, 0, 0xFF, 0xFF, 0xFF, "|");
                break;
            case 3:
                TSKAPI_DISPLAY_TEXT(90, 0, 0xFF, 0xFF, 0xFF, "/");
                break;
        }
        cnt++;
        tslp_tsk(100);
    }
}

