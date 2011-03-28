/**
 * @file task_splash.h
 * @author Shinichiro Nakamura
 * @brief スプラッシュタスクの定義(task_splash)
 * @details
 * よくあるスプラッシュ画面を表示するタスク。
 * このタスクは自身の処理が完了後に、本当に必要なタスクを起動する。
 * 本当に必要なタスクを起動した後は、システムの動作をユーザに通知する
 * だけのタスクとして動作する。
 */

#ifndef TASK_SPLASH_H
#define TASK_SPLASH_H

#define TSKPRI_SPLASH 13
#define TSKSTK_SPLASH STACK_SIZE

/**
 * @defgroup BLACKTANK_LPC1769_TASK_SPLASH
 * @ingroup BLACKTANK_LPC1769_TASK
 * @{
 */

void task_splash(intptr_t exinf);

/**
 * @}
 */

#endif

