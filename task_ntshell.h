/**
 * @file task_ntshell.h
 * @brief ナチュラルタイニーシェルタスク(task_ntshell)
 * システムをコンソールから制御することのできるインターフェースタスクです。
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

#define TSKPRI_NTSHELL    12

/*
 *  関数のプロトタイプ宣言
 */
#ifndef TOPPERS_MACRO_ONLY

extern void	task_ntshell(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */

#endif /* _TASK_NTSHELL_H_ */
