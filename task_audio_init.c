/**
 * \file talkthrough.c
 * \brief Takachihoボードの初期化を行い、他のタスクを起動する。
 */

/**
 * \mainpage
 * LPC1768を使用したAudio信号のTalkthrough (ループバック)デモプログラムである。
 *
 * 内蔵I2SペリフェラルとDMAを使って外部のオーディオ・コーデックからデータを取り込み、内部でコピー
 * して再度オーディオ・コーデックに出力する。内部コピーは \ref process_audio 関数で行っており
 * 内部を書き換えることでどのようなアルゴリズムでも実装できる。
 *
 * \ref task_audio_init は各種の初期化を行った後、 \ref audio_task をアクティブにする。 audio_taskは
 * DMA割り込みハンドラ \ref dma_intr_handler からのシグナルに同期して動作しており、
 * ピンポンバッファを使ってDMAとソフトウェア処理の並列化を図っている。
 *
 * このプログラムが使用しているコーデックはTI社のTLV320AIC23Bである。初期化データ列は
 * \ref codec_init ルーチンから送出している。
 *
 * なお、LPC1768のDMAは、何らかのDMA管理機構を使ってチャンネル割り当てをすべきだが、
 * このプログラムではその操作を省いている。
 */


#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "task_audio_init.h"
#include <LPC17xx.h>
#include "kernel_cfg.h"

/**
 * \param file ソースコードのファイル名
 * \param line ソースコードの行番号
 * \param expr サービスコールのテキスト表現
 * \param ercd サービスコールの結果（エラー値）
 * \brief サービスコールのエラーのログ出力を行う補助関数
 * \details
 * TOPPERS/ASPのサービスコールのエラー出力を行うためのサポート関数。 \ref SVC_PERROR マクロの中から使う。
 *
 */
Inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
	if (ercd < 0) {
		t_perror(LOG_ERROR, file, line, expr, ercd);
	}
}


/**
 * \brief サービスコールのエラー出力マクロ
 * \param expr サービスコールの式
 * \details
 * exprとして与えたサービスコールのソースコード上の表現とその実行結果を印字する。
 * サービスコールに限らず値を持つ式ならなんでもよい。
 */
#define	SVC_PERROR(expr)	svc_perror(__FILE__, __LINE__, #expr, (expr))




/**
 * \brief DMAハンドラ
 * \details
 *  このハンドラは全DMAに対して共通に呼ばれる．
 *
 *  呼ばれると、DMAのバッファ終了割り込みステータスを確認する。それがI2S DMAバッファの
 *  終了割り込みなら、割り込みをクリアしてタスクに通知する。
 *  \todo
 * DMAチャンネルの番号は決め打ちである。適切なDMA管理機構を使った方式への変更が必要である。
 */
void dma_intr_handler(intptr_t exinf)
{
	i2s_dma_intr_handler();
}



/**
 * \brief メインタスク
 * \param exinf コンフィギュレータから受け取る引数。このタスクでは無視する。
 *
 * \details
 * 一連の初期化のあと、オーディオタスクを動かしてダミーループに入る。
 */
void task_audio_init(intptr_t exinf)
{
	int i=0;

	syslog(LOG_NOTICE, "Talkthrough program starts.");

		/* I2C を初期化する */
	i2c_init();
        syslog(LOG_NOTICE, "I2C initialize done.");

		/* AUDIO CODECを初期化する */
	codec_init();
        syslog(LOG_NOTICE, "Codec initialize done.");

		/* ペリフェラル群の初期化 */
	adc_init();
        syslog(LOG_NOTICE, "ADC initialize done.");

	i2s_init();
        syslog(LOG_NOTICE, "I2S initialize done.");

	i2s_dma_init();
        syslog(LOG_NOTICE, "I2S DMA initialize done.");

		/* オーディオ処理タスクをアクティブにする */
	SVC_PERROR(act_tsk(AUDIO_TASK));
        syslog(LOG_NOTICE, "Audio task activated.");

		/* DMAによるI2S転送を始める */
	i2s_start();
        syslog(LOG_NOTICE, "Started I2S.");

		/* ダミーループ */
	while(1)
	{
		tslp_tsk(1000);
		syslog(LOG_NOTICE, "Still Arive : %d", i++);
	}
}

