
#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

#include "kernel_cfg.h"
#include "task_led.h"
#include "debled.h"

#define MSG_CONTROL(n) (((n) & 0x80) ? 0 : 1)
#define MSG_TARGET(n) ((n) & 0x0F)

void task_led(intptr_t exinf)
{
    uint8_t msg;
    while(1)
    {
        while (prcv_dtq(DTQ_LED, (intptr_t *)&msg) == E_OK) {
            switch (MSG_TARGET(msg)) {
                case DBLED0:
                    debled_set(0, MSG_CONTROL(msg));
                    break;
                case DBLED1:
                    debled_set(1, MSG_CONTROL(msg));
                    break;
                case DBLED2:
                    debled_set(2, MSG_CONTROL(msg));
                    break;
                case DBLED3:
                    debled_set(3, MSG_CONTROL(msg));
                    break;
                case SWLED0:
                    break;
                case SWLED1:
                    break;
                case SWLED2:
                    break;
                case SWLED3:
                    break;
            }
        }
        tslp_tsk(10);
    }
}

