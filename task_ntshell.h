/**
 * @file task_ntshell.h
 * @author Shinichiro Nakamura
 * @brief ナチュラルタイニーシェルタスクの定義(task_ntshell)
 * @details
 * システムをコンソールから制御することのできるインターフェースタスク。
 * 主に開発者のために提供される。
 */

#ifndef _TASK_NTSHELL_H_
#define _TASK_NTSHELL_H_

/*
 *  ターゲット依存の定義
 */
#include "target_test.h"

/*
 *  各タスクの優先度の定義
 */

#define TSKPRI_NTSHELL    14
#define TSKSTK_NTSHELL    STACK_SIZE

/*
 *  関数のプロトタイプ宣言
 */
#ifndef TOPPERS_MACRO_ONLY

extern void	task_ntshell(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */

#endif /* _TASK_NTSHELL_H_ */
