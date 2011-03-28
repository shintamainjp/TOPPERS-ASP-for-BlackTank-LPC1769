/**
 * @file task_led.h
 * @author Shinichiro Nakamura
 * @brief LEDタスクの定義(task_led)
 * @details
 * LEDタスクはLEDの点灯を制御するタスクである。
 * 点灯は各タスクから次のマクロのみで指示することができる。
 *
 * @code
 * LEDMSG(target, control);
 * @endcode
 */

#ifndef _TASK_LED_H_
#define _TASK_LED_H_

#include "target_test.h"

#define TSKPRI_LED  11
#define TSKSTK_LED  STACK_SIZE

#ifndef TOPPERS_MACRO_ONLY

#define DBLED0 0
#define DBLED1 1
#define DBLED2 2
#define DBLED3 3
#define SWLED0 4
#define SWLED1 5
#define SWLED2 6
#define SWLED3 7

#define LEDOFF 0
#define LEDON 1

#define LEDMSG(target, control) \
    snd_dtq(DTQ_LED, (intptr_t)(((control) ? 0x80:0x00) | (target & 0x0F)))

/**
 * @defgroup BLACKTANK_LPC1769_TASK_LED
 * @ingroup BLACKTANK_LPC1769_TASK
 * @{
 */

void task_led(intptr_t exinf);

/**
 * @}
 */

#endif

#endif
