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

#define DEVICE_SW0 0    /**< デバイス：スイッチ０ */
#define DEVICE_SW1 1    /**< デバイス：スイッチ１ */
#define DEVICE_SW2 2    /**< デバイス：スイッチ２ */
#define DEVICE_SW3 3    /**< デバイス：スイッチ３ */
#define DEVICE_VOL0 4   /**< デバイス：ボリューム０ */
#define DEVICE_VOL1 5   /**< デバイス：ボリューム１ */
#define DEVICE_VOL2 6   /**< デバイス：ボリューム２ */
#define DEVICE_VOL3 7   /**< デバイス：ボリューム３ */

/**
 * @brief menuタスクAPI（ユーザメッセージ）の実行。
 * @param device デバイス番号。
 * @param value 値。
 */
#define TSKAPI_MENU_USERMSG(device, value) \
    snd_dtq(DTQ_USERINPUT, \
            (intptr_t)(((device & 0x000F) << 12) | ((value & 0x0FFF) << 0)))

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

