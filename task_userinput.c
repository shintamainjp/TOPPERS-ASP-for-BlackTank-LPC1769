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
        } else {
            LEDMSG(DBLED0, LEDOFF);
        }
        if (hid_swread(1)) {
            LEDMSG(DBLED1, LEDON);
        } else {
            LEDMSG(DBLED1, LEDOFF);
        }
        if (hid_swread(2)) {
            LEDMSG(DBLED2, LEDON);
        } else {
            LEDMSG(DBLED2, LEDOFF);
        }
        if (hid_swread(3)) {
            LEDMSG(DBLED3, LEDON);
        } else {
            LEDMSG(DBLED3, LEDOFF);
        }
        tslp_tsk(50);
    }
}

