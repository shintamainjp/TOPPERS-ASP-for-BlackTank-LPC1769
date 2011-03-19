#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>
#include <itron.h>

#include "kernel_cfg.h"
#include "task_menu.h"
#include "task_led.h"
#include "task_display.h"

#define MSG_DEVICE(n) (((n) & 0xE0) >> 5)
#define MSG_VALUE(n) ((n) & 0x1F)

void task_menu(intptr_t exinf)
{
    uint8_t msg;
    while(1)
    {
        while (prcv_dtq(DTQ_USERINPUT, (intptr_t *)&msg) == E_OK) {
            switch (MSG_DEVICE(msg)) {
                case SW0:
                    if (MSG_VALUE(msg)) {
                        LEDMSG(DBLED0, LEDON);
                    } else {
                        LEDMSG(DBLED0, LEDOFF);
                    }
                    DISP_CLEAR(0xff, 0, 0);
                    break;
                case SW1:
                    if (MSG_VALUE(msg)) {
                        LEDMSG(DBLED1, LEDON);
                    } else {
                        LEDMSG(DBLED1, LEDOFF);
                    }
                    DISP_CLEAR(0, 0xff, 0);
                    break;
                case SW2:
                    if (MSG_VALUE(msg)) {
                        LEDMSG(DBLED2, LEDON);
                    } else {
                        LEDMSG(DBLED2, LEDOFF);
                    }
                    DISP_CLEAR(0, 0, 0xff);
                    break;
                case SW3:
                    if (MSG_VALUE(msg)) {
                        LEDMSG(DBLED3, LEDON);
                    } else {
                        LEDMSG(DBLED3, LEDOFF);
                    }
                    DISP_CLEAR(0, 0, 0);
                    break;
                case VOL0:
                    if (MSG_VALUE(msg) > 16) {
                        LEDMSG(SWLED0, LEDON);
                    } else {
                        LEDMSG(SWLED0, LEDOFF);
                    }
                    break;
                case VOL1:
                    if (MSG_VALUE(msg) > 16) {
                        LEDMSG(SWLED1, LEDON);
                    } else {
                        LEDMSG(SWLED1, LEDOFF);
                    }
                    break;
                case VOL2:
                    if (MSG_VALUE(msg) > 16) {
                        LEDMSG(SWLED2, LEDON);
                    } else {
                        LEDMSG(SWLED2, LEDOFF);
                    }
                    break;
                case VOL3:
                    if (MSG_VALUE(msg) > 16) {
                        LEDMSG(SWLED3, LEDON);
                    } else {
                        LEDMSG(SWLED3, LEDOFF);
                    }
                    break;
            }
        }
        tslp_tsk(40);
    }
}

