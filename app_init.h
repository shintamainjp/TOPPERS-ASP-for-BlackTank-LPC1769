/**
 * \file ledblink.h
 * \brief アプリケーション用インクルードファイル。コンフィギュレータも読み込む。
 */
#ifndef _APP_INIT_H_
#define _APP_INIT_H_

/*
 *  ターゲット依存の定義
 */
#include "target_test.h"

/*
 *  関数のプロトタイプ宣言
 */
#ifndef TOPPERS_MACRO_ONLY

extern void	app_init(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */

#endif /* _APP_INIT_H_ */

