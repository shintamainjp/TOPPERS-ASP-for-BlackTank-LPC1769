/**
 * @file task_userinput.h
 * @author Shinichiro Nakamura
 * @brief ユーザインプットタスクの定義(task_userinput)
 * @details
 * ユーザインプットタスクは４系統のユーザ入力の変化を観察するタスク。
 * 変化があればデータキューを介してメニュータスクに通知を行う。
 */

#ifndef _TASK_USERINPUT_H_
#define _TASK_USERINPUT_H_

#include "target_test.h"

/**
 * @brief タスクプライオリティ。(ユーザ入力タスク)
 */
#define TSKPRI_USERINPUT    12
#define TSKSTK_USERINPUT    STACK_SIZE

#ifndef TOPPERS_MACRO_ONLY

/**
 * @defgroup BLACKTANK_LPC1769_TASK_USERINPUT
 * @ingroup BLACKTANK_LPC1769_TASK
 * @{
 */

void task_userinput(intptr_t exinf);

/**
 * @}
 */

#endif

#endif

