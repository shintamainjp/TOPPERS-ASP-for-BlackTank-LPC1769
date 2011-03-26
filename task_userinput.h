/**
 * @file task_userinput.h
 * @brief ユーザインプットタスク(task_userinput)
 * ユーザインプットタスクは４系統のユーザ入力の変化を観察するタスクです。
 * 変化があればデータキューを介してメニュータスクに通知されます。
 */

#ifndef _TASK_USERINPUT_H_
#define _TASK_USERINPUT_H_

#include "target_test.h"

#define TSKPRI_USERINPUT    12
#define TSKSTK_USERINPUT    STACK_SIZE

#ifndef TOPPERS_MACRO_ONLY

void task_userinput(intptr_t exinf);

#endif

#endif

