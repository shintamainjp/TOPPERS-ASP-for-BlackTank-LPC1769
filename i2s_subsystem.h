/**
 * @file i2s_subsystem.h
 * @brief I2S操作関数群
 * @date 2011/02/19
 * @author Takemasa Nakamura
 * @details
 * @ref i2s_init 関数を使ってI2Sペリフェラルを初期化し、
 * 次に @ref i2s_dma_init 関数を使ってDMA機能を初期化する。
 * 最後に @ref i2s_start 関数を呼ぶことで実際のI2S動作が始まる。
 *
 * I2Sはダブルバッファを使う。
 * バッファのデータ転送が終わるたびにDMA割り込みが入る。
 * この割り込みを処理するにはDMA割り込みが起きるたびに、
 * @ref i2s_dma_intr_handler 関数を割り込みハンドラ内部から呼べばよい。
 *
 * @ref audio_task 関数は、オーディオ処理タスクの実態であり、
 * TOPPERS/ASPのタスクにすること。
 * この関数はSEM_I2SDMAセマフォを使って @ref i2s_dma_intr_handler と
 * 同期するため、SEM_I2SDMAセマフォを用意しておかなければならない。
 *
 * @ref process_audio 関数は、audio_processing.c でユーザーが実装すべき
 * オーディオ信号処理関数である。この関数は audio_task から呼び出される。
 */

#ifndef I2S_SUBSYSTEM_H_
#define I2S_SUBSYSTEM_H_

#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>
#include "audio_common.h"

/**
 * @brief DMA用のリンクリストの構成要素型
 * @details
 * LP17xxの GPDMAはスキャッタ、ギャザーにも使用できるリンクリストを
 * 構成できる。LLI型はそのリストの構成単位となるデータ型である。
 * 基本的にLLIはLPC1768のユーザーズマニュアル、UM10360で記述されている
 * DMA LLIそのままである。
 * 詳しくはユーザーズマニュアル参照の事。
 */
struct LLI {
    int * SrcAddr;  /**< DMAのソース・バッファ・アドレス */
    int * DstAddr;  /**< DMAのデスティネーション・バッファ・アドレス・ */
    struct LLI * nextLLI;   /**< 次のLLI要素へのポインタ */
    unsigned int Control;   /**< DMAコントロール・レジスタへのロード値 */
};

/**
 * @brief I2S用データ
 * @details DMA転送などに使うデータを一ヶ所にあつめたもの。
 */
struct I2S_AUDIO_DATA {
    /**
     * @brief 送信DMA用のLLIリスト
     * @details
     * LLIチェーンは2要素をもち、全体でサーキュラーバッファを構成する。
     * 初期化は @ref i2s_dma_init で行う。
     */
    struct LLI txI2SLLI[2];

    /**
     * @brief 受信DMA用のLLIリスト
     * @details
     * LLIチェーンは2要素をもち、全体でサーキュラーバッファを構成する。
     * 初期化は @ref i2s_dma_init で行う。
     */
    struct LLI rxI2SLLI[2];

    /**
     * @brief I2S送信DMAのバッファ
     * @details
     * DMAを使ってI2Sポートから送信するためのデータバッファ。
     * 二つのバッファを持つのは、ピンポン（ダブルバッファ）
     * 制御を行うためである。
     * DMA転送中にプログラムが次の送信データを書き込むためのバッファを
     * 取得するには、
     * @ref i2s_getTxBuf 関数を使う。
     */
    AUDIOSAMPLE txBuffer[2][AUDIOBUFSIZE];

    /**
     * @brief I2S受信DMAのバッファ
     * @details
     * DMAを使ってI2Sポートから受信するためのデータバッファ。
     * 二つのバッファを持つのは、ピンポン（ダブルバッファ）
     * 制御を行うためである。
     * DMA転送中にプログラムが直前の受信データを読み込むための
     * バッファを取得するには、 @ref i2s_getRxBuf 関数を使う。
     */
    AUDIOSAMPLE rxBuffer[2][AUDIOBUFSIZE];
};

/**
 * @brief I2S 初期化ルーチン
 * @details
 * LPC1768のI2Sペリフェラルを初期化する。送受信とも
 * @li スレーブ
 * @li 32bit
 * @li 4ワイアー
 * である。DMA1をRXに、DMA2をTXに割り当てている。
 *
 * なお、このルーチンは初期化はするが、I2Sを起動しない。
 * 起動はDMAの設定が終わった後に @ref i2s_start ルーチンを呼んで行う。
 *
 * @todo
 * 現在の設定でも過不足なく動くが、TXのクロックをRXから分けるようにすれば、
 * もっと少ないピン数で動作させることもできるはずである。
 */
void i2s_init();

/**
 * @brief DMA初期化
 * @details
 * I2Sに関連づけされたDMAを初期化する。
 * 同時に、LLIを初期化して、ピンポンバッファをすぐに使えるような
 * 状態にする。
 * 受信DMAは各バッファの受信完了ごとにDMA割り込みをおこす。
 * 送信では起こさない。
 * プログラムはこれに同期してタスクと協調動作をする。
 *
 * DMA受信割り込みは @ref dma_intr_handler で処理を行う。
 *
 * @todo
 * DMAの割り当ては決め打ちである。
 * 適切なDMA管理機構を使用して衝突を避ける。
 */
void i2s_dma_init();

/**
 * @brief I2S 開始。DMA初期化後に行う
 * @details
 * @ref i2s_init ルーチンと @ref i2s_dma_init ルーチンによって準備を
 * 終えた後、このルーチンを呼び出してi2sによる転送を始める。
 */
void i2s_start();

/**
 * @brief I2S DMA　割り込みハンドラの初期化を行う
 * @details
 * LPC1768のDMA割り込みステータスをチェックし、I2S DMA 受信TC割り込みが
 * 発生していたら、その割り込みに対処する。
 */
void i2s_dma_intr_handler();

AUDIOSAMPLE * i2s_getTxBuf();
AUDIOSAMPLE * i2s_getRxBuf();

#endif

