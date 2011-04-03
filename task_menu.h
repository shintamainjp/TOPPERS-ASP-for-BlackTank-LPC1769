/**
 * @file task_menu.h
 * @author Shinichiro Nakamura
 * @brief メニュータスクの定義(task_menu)
 * @details
 * メニュータスクはユーザにシステムのサービスを提示し、
 * ユーザからの要求をシステムに伝達する役目を果たす。
 * ユーザからの入力はそれらを管理するタスクからの通知に依存する。
 */

#ifndef _TASK_MENU_H_
#define _TASK_MENU_H_

#include "target_test.h"

/**
 * @brief タスクプライオリティ。(メニュータスク)
 */
#define TSKPRI_MENU    12
#define TSKSTK_MENU    STACK_SIZE

#ifndef TOPPERS_MACRO_ONLY

/**
 * @defgroup BLACKTANK_LPC1769_TASK_MENU_CLIENT
 * @ingroup BLACKTANK_LPC1769_TASK_MENU
 * @{
 */

#define SW0 0
#define SW1 1
#define SW2 2
#define SW3 3
#define VOL0 4
#define VOL1 5
#define VOL2 6
#define VOL3 7

#define SW_LONG_PUSH 0x01

#define USERMSG(device, type, value) \
    snd_dtq(DTQ_USERINPUT, \
            (intptr_t)( \
                ((device & 0x000F) << 12) \
                | ((type & 0x0003) << 10) \
                | ((value & 0x03FF) << 0)))

/**
 * @}
 */

/**
 * @defgroup BLACKTANK_LPC1769_TASK_MENU
 * @ingroup BLACKTANK_LPC1769_TASK
 * @{
 */

void task_menu(intptr_t exinf);

/**
 * @}
 */

#endif

#endif

