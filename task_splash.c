
#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>

#include "task_splash.h"
#include "kernel_cfg.h"
#include "task_display.h"

void task_splash(intptr_t exinf)
{
    tslp_tsk(500);

    DISP_CLEAR(0x00, 0x00, 0x00);
    tslp_tsk(500);

    DISP_TEXT(0, 10 * 0, 0xFF, 0xFF, 0xFF, "Special thanks to");
    tslp_tsk(500);

    DISP_TEXT(0, 10 * 1, 0xFF, 0xFF, 0xFF, "Horie-san");
    tslp_tsk(500);

    DISP_TEXT(0, 10 * 2, 0xFF, 0xFF, 0xFF, "Kaneko-san");
    tslp_tsk(500);

    tslp_tsk(1000);

    DISP_BMPFILE("0:LOGO.BMP");
    tslp_tsk(2000);

    act_tsk(TASK_USERINPUT);
    act_tsk(TASK_MENU);
    act_tsk(TASK_AUDIO_INIT);

    ext_tsk();
}

