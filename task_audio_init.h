/**
 * \file talkthrough.h
 * \brief アプリケーション・インクルード ファイル
 */

#ifndef TAKACHIHO_H_
#define TAKACHIHO_H_

#include "i2s_subsystem.h"
#include "i2c_subsystem.h"
#include "adc_subsystem.h"
#include "codec_subsystem.h"

/*
 *  ターゲット依存の定義
 */
#include "target_test.h"

/*
 *  各タスクの優先度の定義
 *  .cfg ファイルで参照する
 */

#define TSKPRI_AUDIO_INIT 16
#define AUDIO_PRIORITY 10

/*
 *  ターゲットに依存する可能性のある定数の定義
 *
 *  .cfgファイルで参照する
 */

#ifndef TASK_PORTID
#define TASK_PORTID		1			/* 文字入力するシリアルポートID */
#endif /* TASK_PORTID */


/*
 *  関数のプロトタイプ宣言
 *
 *  .cfg ファイルで参照する
 */
#ifndef TOPPERS_MACRO_ONLY

extern void task_audio_init(intptr_t exinf);
extern void audio_task(intptr_t exinf);
extern void dma_intr_handler(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TAKACHIHO_H_ */
