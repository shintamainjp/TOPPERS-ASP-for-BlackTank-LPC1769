/**
 * @file task_led.c
 * @author Shinichiro Nakamura
 * @brief LEDタスクの実装(task_led)
 * @details
 * LEDタスクはLEDの点灯を制御するタスクである。
 * 点灯は各タスクから次のマクロのみで指示することができる。
 * LEDMSG(target, control);
 */

#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

#include "kernel_cfg.h"
#include "task_led.h"
#include "led.h"

#define MSG_CONTROL(n) (((n) & 0x0080) ? 1 : 0)
#define MSG_TARGET(n) ((n) & 0x000F)

void task_led(intptr_t exinf)
{
    uint16_t msg;
    led_init();
    while(1)
    {
        if (rcv_dtq(DTQ_LED, (intptr_t *)&msg) == E_OK) {
            switch (MSG_TARGET(msg)) {
                case DBLED0:
                    led_debug_write(0, MSG_CONTROL(msg));
                    break;
                case DBLED1:
                    led_debug_write(1, MSG_CONTROL(msg));
                    break;
                case DBLED2:
                    led_debug_write(2, MSG_CONTROL(msg));
                    break;
                case DBLED3:
                    led_debug_write(3, MSG_CONTROL(msg));
                    break;
                case SWLED0:
                    led_switch_write(0, MSG_CONTROL(msg));
                    break;
                case SWLED1:
                    led_switch_write(1, MSG_CONTROL(msg));
                    break;
                case SWLED2:
                    led_switch_write(2, MSG_CONTROL(msg));
                    break;
                case SWLED3:
                    led_switch_write(3, MSG_CONTROL(msg));
                    break;
            }
        }
        tslp_tsk(10);
    }
}

