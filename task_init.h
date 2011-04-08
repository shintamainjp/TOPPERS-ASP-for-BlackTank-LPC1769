/**
 * @file task_init.h
 * @author Shinichiro Nakamura
 * @brief 初期化タスクの定義(task_init)
 * @details
 * よくある初期化画面を表示するタスク。
 * このタスクは自身の処理が完了後に、本当に必要なタスクを起動する。
 * 本当に必要なタスクを起動した後は、システムの動作をユーザに通知する
 * だけのタスクとして動作する。
 */

#ifndef TASK_INIT_H
#define TASK_INIT_H

/**
 * @brief タスクプライオリティ。(初期化)
 */
#define TSKPRI_INIT 13
#define TSKSTK_INIT STACK_SIZE

/**
 * @defgroup BLACKTANK_LPC1769_TASK_INIT
 * @ingroup BLACKTANK_LPC1769_TASK
 * @{
 */

void task_init(intptr_t exinf);

/**
 * @}
 */

#endif

