/**
 * @file sys_init.h
 * @author Shinichiro Nakamura
 * @brief システムで優先的に初期化したい処理の定義。
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

