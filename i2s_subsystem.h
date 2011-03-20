/**
 * \file i2s_subsystem.h
 * \brief I2S操作関数群
 * \date 2011/02/19
 * \author Takemasa Nakamura
 * \details
 * \ref i2s_init 関数を使ってI2Sペリフェラルを初期化し、次に \ref i2s_dma_init 関数を
 * 使ってDMA機能を初期化する。最後に \ref i2s_start 関数を呼ぶことで実際のI2S動作が
 * 始まる。
 *
 * I2Sはダブルバッファを使う。バッファのデータ転送が終わるたびにDMA割り込みが入る。
 * この割り込みを処理するにはDMA割り込みが起きるたびに、 \ref i2s_dma_intr_handler 関数を
 * 割り込みハンドラ内部から呼べばよい。
 *
 * \ref audio_task 関数は、オーディオ処理タスクの実態であり、TOPPERS/ASPのタスクに
 * すること。この関数はSEM_I2SDMAセマフォを使って \ref i2s_dma_intr_handler と
 * 同期するため、SEM_I2SDMAセマフォを用意しておかなければならない。
 *
 * \ref process_audio　関数は、audio_processing.c でユーザーが実装すべき
 * オーディオ信号処理関数である。この関数は audio_task から呼び出される。
 */


#ifndef I2S_SUBSYSTEM_H_
#define I2S_SUBSYSTEM_H_

#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

/**
 * \brief オーディオデータ型
 * \details
 * プログラム内部で使うオーディオの基本型。左詰めの符号付き固定小数点型である。小数点はMSBの
 * すぐ右にある。
 */
typedef int AUDIOSAMPLE;

/**
 * \brief I2S DMAバッファの長さ
 * \details
 * 一回のDMA転送に使うデータバッファの長さ。AUDIOBUFSIZEがNならば、ステレオなので、
 * N/2サンプルのデータを一回のDMAで転送することになる。
 */
#define	AUDIOBUFSIZE	64	 /* I2Sバッファの長さ。48kHzサンプルの時、1mSにするには96を選ぶ */


/**
 * \brief I2S 初期化ルーチン
 * \details
 * LPC1768のI2Sペリフェラルを初期化する。送受信とも
 * \li スレーブ
 * \li 32bit
 * \li 4ワイアー
 * である。DMA1をRXに、DMA2をTXに割り当てている。
 *
 * なお、このルーチンは初期化はするが、I2Sを
 * 起動しない。起動はDMAの設定が終わった後に \ref i2s_start ルーチンを呼んで行う。
 * \todo
 * 現在の設定でも過不足なく動くが、TXのクロックを * RXから分けるようにすれば、
 * もっと少ないピン数で動作させることもできるはずである。
 */
void i2s_init();

/**
 * \brief DMA初期化
 * \details
 * I2Sに関連づけされたDMAを初期化する。同時に、LLIを初期化して、ピンポンバッファをすぐに使えるような
 * 状態にする。受信DMAは各バッファの受信完了ごとにDMA割り込みをおこす。送信では起こさない。プログラムは
 * これに同期してタスクと協調動作をする。
 *
 * DMA受信割り込みは \ref dma_intr_handler で処理を行う。
 * \todo
 * DMAの割り当ては決め打ちである。適切なDMA管理機構を使用して衝突を避ける。
 *
 */

void i2s_dma_init();

/**
 * \brief I2S 開始。DMA初期化後に行う
 * \details
 * \ref i2s_init ルーチンと \ref i2s_dma_init ルーチンによって準備を終えた後、
 * このルーチンを呼び出してi2sによる転送を始める。
 */

void i2s_start();

/**
 * \brief I2S DMA　割り込みハンドラの初期化を行う
 * \details
 * LPC1768のDMA割り込みステータスをチェックし、I2S DMA 受信TC割り込みが発生していたら、その割り込みに対処する。
 */
void i2s_dma_intr_handler();


/**
 * \brief オーディオ信号処理関数
 * \param input 入力のオーディオデータ列。LchとRchに分かれて配列になっている
 * \param output 出力のオーディオデータ列。LchとRchに分かれて配列になっている
 * \param count 入出力データオンサンプル数。count=8のとき、入出力のデータ数はステレオなのでそれぞれ16となる。
 * \details
 * ユーザー信号処理を行う関数である。入力はあらかじめLとRに分離されて関数に渡される。同様に出力は
 * LとRを分離して受け取る。配列の添字としては、Lが0、Rが1である。
 *
 * countはサンプル数を表す。
 *
 */
void process_audio( AUDIOSAMPLE input[2][AUDIOBUFSIZE/2], AUDIOSAMPLE output[2][AUDIOBUFSIZE/2], int count );


/**
 * \brief オーディオDMAに同期して動くタスク
 * \param exinf コンフィギュレータがタスクに引き渡す変数。このタスクは無視する。
 * \details
 * \ref dma_intr_handler からのSEM_I2SDMA セマフォ・シグナルを受けて
 * しかるべきデータ・バッファを特定し、信号処理を行う。
 *
 * なお、このタスクはシステム起動時にはインアクティブである。
 * \ref main_task から明示的にアクティベートして始めて動作を開始する。
 *
 */
void audio_task(intptr_t exinf);


#endif /* I2S_SUBSYSTEM_H_ */
