#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

#include "kernel_cfg.h"
#include "task_userinput.h"
#include "task_led.h"
#include "hid.h"

void task_userinput(intptr_t exinf)
{
    hid_init();
    while(1)
    {
        if (hid_swread(0)) {
            LEDMSG(DBLED0, LEDON);
            LEDMSG(SWLED0, LEDON);
        } else {
            LEDMSG(DBLED0, LEDOFF);
            LEDMSG(SWLED0, LEDOFF);
        }
        if (hid_swread(1)) {
            LEDMSG(DBLED1, LEDON);
            LEDMSG(SWLED1, LEDON);
        } else {
            LEDMSG(DBLED1, LEDOFF);
            LEDMSG(SWLED1, LEDOFF);
        }
        if (hid_swread(2)) {
            LEDMSG(DBLED2, LEDON);
            LEDMSG(SWLED2, LEDON);
        } else {
            LEDMSG(DBLED2, LEDOFF);
            LEDMSG(SWLED2, LEDOFF);
        }
        if (hid_swread(3)) {
            LEDMSG(DBLED3, LEDON);
            LEDMSG(SWLED3, LEDON);
        } else {
            LEDMSG(DBLED3, LEDOFF);
            LEDMSG(SWLED3, LEDOFF);
        }
        tslp_tsk(50);
    }
}

