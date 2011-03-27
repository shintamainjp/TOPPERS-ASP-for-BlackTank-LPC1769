
#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>

#include "task_splash.h"
#include "kernel_cfg.h"
#include "task_display.h"
#include "config.h"

void task_splash(intptr_t exinf)
{
    tslp_tsk(500);

    DISP_BMPFILE("0:LOGO.BMP");
    tslp_tsk(2500);

    DISP_CLEAR(0x00, 0x00, 0x00);
    tslp_tsk(500);

    static const int XOFS = 5;
    static const int YOFS = 10;
    static const int YSTP = 12;

    DISP_TEXT(XOFS + 0, YOFS + YSTP * 0,
            0xFF, 0xFF, 0xFF, "Special thanks to");
    tslp_tsk(500);

    DISP_TEXT(XOFS + 22, YOFS + YSTP * 2,
            0xFF, 0xFF, 0xFF, "Horie-san");
    tslp_tsk(500);

    DISP_TEXT(XOFS + 20, YOFS + YSTP * 3,
            0xFF, 0xFF, 0xFF, "Kaneko-san");
    tslp_tsk(500);

    tslp_tsk(2000);

    act_tsk(TASK_MENU);
    act_tsk(TASK_USERINPUT);
    tslp_tsk(500);

    if (TASK_AUDIO_ENABLED()) {
        /*
         * メニューが表示された頃合いを見てオーディオ処理を開始する。
         */
        act_tsk(TASK_AUDIO);
    } else {
        /*
         * オーディオ機能オフ.
         */
        syslog(LOG_NOTICE, "task_audio disabled in this build.");
    }

    int cnt = 0;
    while (1) {
        /*
         * 動作を示すマーキング.
         */
        DISP_FILLBOX(90, 0, 95, 6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
        switch (cnt % 4) {
            case 0:
                DISP_TEXT(90, 0, 0xFF, 0xFF, 0xFF, "-");
                break;
            case 1:
                DISP_TEXT(90, 0, 0xFF, 0xFF, 0xFF, "\\");
                break;
            case 2:
                DISP_TEXT(90, 0, 0xFF, 0xFF, 0xFF, "|");
                break;
            case 3:
                DISP_TEXT(90, 0, 0xFF, 0xFF, 0xFF, "/");
                break;
        }
        cnt++;
        tslp_tsk(100);
    }
}

