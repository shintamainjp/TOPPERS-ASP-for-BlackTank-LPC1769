/**
 * @file ledblink.h
 * @brief アプリケーション用インクルードファイル。
 * コンフィギュレータも読み込む。
 */
#ifndef _SYS_INIT_H_
#define _SYS_INIT_H_

/*
 *  ターゲット依存の定義
 */
#include "target_test.h"

/*
 *  関数のプロトタイプ宣言
 */
#ifndef TOPPERS_MACRO_ONLY

void sys_init(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */

#endif /* _SYS_INIT_H_ */

