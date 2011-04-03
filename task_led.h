/**
 * @file task_led.h
 * @author Shinichiro Nakamura
 * @brief LEDタスクの定義(task_led)
 * @details
 * LEDタスクはLEDの点灯を制御するタスクである。
 * 点灯は各タスクからマクロで指示することができる。
 */

/**
 * @example
 * LEDの点灯指令は以下のマクロを使って行うことができる。
 *
 * @code
 * LEDMSG(target, control);
 * @endcode
 *
 * このマクロは、タスクコンテキスト内のどこからでも呼ぶことができる。
 */

#ifndef _TASK_LED_H_
#define _TASK_LED_H_

#include "target_test.h"

/**
 * @brief タスクプライオリティ。(LEDタスク)
 */
#define TSKPRI_LED  11
#define TSKSTK_LED  STACK_SIZE

#ifndef TOPPERS_MACRO_ONLY

/**
 * @defgroup BLACKTANK_LPC1769_TASK_LED_CLIENT
 * @ingroup BLACKTANK_LPC1769_TASK_LED
 * @{
 */

#define DBLED0 0    /**< デバッグ用LED0 */
#define DBLED1 1    /**< デバッグ用LED1 */
#define DBLED2 2    /**< デバッグ用LED2 */
#define DBLED3 3    /**< デバッグ用LED3 */
#define SWLED0 4    /**< スイッチ用LED0 */
#define SWLED1 5    /**< スイッチ用LED1 */
#define SWLED2 6    /**< スイッチ用LED2 */
#define SWLED3 7    /**< スイッチ用LED3 */

#define LEDOFF 0    /**< LEDを消灯する。 */
#define LEDON 1     /**< LEDを点灯する。 */

#define LEDMSG(TARGET, CONTROL) \
    snd_dtq(DTQ_LED, (intptr_t)(((CONTROL) ? 0x80:0x00) | ((TARGET)& 0x0F)))
/**<
 * LEDの点灯指令を行う。
 *
 * @param TARGET ターゲット。
 * @param CONTROL 制御内容。
 */

/**
 * @}
 */

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
