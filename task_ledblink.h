/**
 * \file task_ledblink.h
 * \brief アプリケーション用インクルードファイル。コンフィギュレータも読み込む。
 */
#ifndef _TASK_LEDBLINK_H_
#define _TASK_LEDBLINK_H_

/*
 *  ターゲット依存の定義
 */
#include "target_test.h"

/*
 *  各タスクの優先度の定義
 */

#define TSKPRI_LEDBLINK 5		/* メインタスクの優先度 */

/*
 *  関数のプロトタイプ宣言
 */
#ifndef TOPPERS_MACRO_ONLY

extern void	task_ledblink(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */

#endif /* _TASK_LEDBLINK_H_ */
