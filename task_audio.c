/**
 * @file task_audio.c
 * @brief オーディオDMAに同期して動くタスク
 * @details
 * @ref dma_intr_handler からのSEM_I2SDMA セマフォ・シグナルを受けて
 * しかるべきデータ・バッファを特定し、信号処理を行う。
 *
 * なお、このタスクはシステム起動時にはインアクティブである。
 * 明示的にアクティベートして始めて動作を開始する。
 *
 * 内蔵I2SペリフェラルとDMAを使って外部のオーディオ・コーデック
 * からデータを取り込み、内部でコピーして再度オーディオ・コーデックに
 * 出力する。内部コピーは \ref process_audio 関数で行っており
 * 内部を書き換えることでどのようなアルゴリズムでも実装できる。
 *
 * audio_taskはDMA割り込みハンドラ \ref dma_intr_handler からの
 * シグナルに同期して動作し、ピンポンバッファを使ってDMAと
 * ソフトウェア処理の並列化を図っている。
 *
 * このプログラムが使用しているコーデックはTI社のTLV320AIC23Bである。
 * 初期化データ列は \ref codec_init ルーチンから送出している。
 *
 * なお、LPC1768のDMAは、何らかのDMA管理機構を使ってチャンネル割り当てを
 * すべきだが、このプログラムではその操作を省いている。
 */

#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include <LPC17xx.h>
#include "kernel_cfg.h"
#include "audio_common.h"
#include "audio_effect.h"
#include "i2s_subsystem.h"
#include "i2c_subsystem.h"
#include "codec_subsystem.h"
#include "testpin.h"
#include "task_audio.h"
#include "task_display.h"

#define MSG_TARGET(n) (((n) & 0xF000) >> 12)
#define MSG_VALUE(n)  (((n) & 0x0FFF) >>  0)

/**
 * \param file ソースコードのファイル名
 * \param line ソースコードの行番号
 * \param expr サービスコールのテキスト表現
 * \param ercd サービスコールの結果（エラー値）
 * \brief サービスコールのエラーのログ出力を行う補助関数
 * \details
 * TOPPERS/ASPのサービスコールのエラー出力を行うためのサポート関数。
 * \ref SVC_PERROR マクロの中から使う。
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
 * exprとして与えたサービスコールのソースコード上の表現と
 * その実行結果を印字する。
 * サービスコールに限らず値を持つ式ならなんでもよい。
 */
#define	SVC_PERROR(expr) svc_perror(__FILE__, __LINE__, #expr, (expr))

/**
 * \brief DMAハンドラ
 * \details
 * このハンドラは全DMAに対して共通に呼ばれる．
 *
 * 呼ばれると、DMAのバッファ終了割り込みステータスを確認する。
 * それがI2S DMAバッファの終了割り込みなら、割り込みをクリアして
 * タスクに通知する。
 * \todo
 * DMAチャンネルの番号は決め打ちである。
 * 適切なDMA管理機構を使った方式への変更が必要である。
 */
void dma_intr_handler(intptr_t exinf)
{
    i2s_dma_intr_handler();
}

/**
 * \brief I2S用データ
 * \details
 * DMAのLLIやバッファなどはすべてこの変数にパッケージしている。
 * こうすることで、グローバル空間に名前が散らかることを阻止できる。
 */
struct I2S_AUDIO_DATA audio_data;

struct I2S_AUDIO_DATA* get_audio_data(void)
{
    return &audio_data;
}

void task_audio(intptr_t exinf)
{
    AUDIOSAMPLE *txbuf;
    const AUDIOSAMPLE *rxbuf;
    int index;
    AUDIOSAMPLE lm_left, lm_right;
    uint16_t msg;
    effect_param_t effect_param;
    void (*effect_func)(
            const effect_param_t *param,
            const AUDIOSAMPLE *in_left,
            const AUDIOSAMPLE *in_right,
            AUDIOSAMPLE *out_left,
            AUDIOSAMPLE *out_right);

    /*
     * オーディオコーデックを初期化する。
     */
    i2c_init();
    codec_init();

    /*
     * ペリフェラルを初期化する。
     */
    i2s_init();
    i2s_dma_init(get_audio_data());

    /*
     * @todo
     * i2s_start()を呼ぶとDMA割り込みの無限ループに陥る。
     * 現象が発生するのはコールドスタートの時だけ。要調査。
     */
    i2s_start();

    /*
     * リアルタイム・ステータス用のテストピンを出力にする。
     */
    testpin_init();

    effect_func = audio_effect_through;

    while(1)
    {
        // 外部タスクからパラメータを取得する。
        if (prcv_dtq(DTQ_AUDIOPARAM, (intptr_t *)&msg) == E_OK) {
            switch (MSG_TARGET(msg)) {
                case AUDIO_PARAM_MODE:
                    switch (MSG_VALUE(msg)) {
                        case AUDIO_VALUE_MODE_THROUGH:
                            effect_func = audio_effect_through;
                            break;
                        case AUDIO_VALUE_MODE_VOCAL_CANCEL:
                            effect_func = audio_effect_vocal_cancel;
                            break;
                        case AUDIO_VALUE_MODE_FIR:
                            effect_func = audio_effect_fir;
                            break;
                        case AUDIO_VALUE_MODE_IIR:
                            effect_func = audio_effect_iir;
                            break;
                        default:
                            syslog(LOG_NOTICE,
                                    "Unknown effect mode %d.",
                                    MSG_VALUE(msg));
                            break;
                    }
                    break;
                case AUDIO_PARAM_VAR0:
                    effect_param.var0 = MSG_VALUE(msg);
                    break;
                case AUDIO_PARAM_VAR1:
                    effect_param.var1 = MSG_VALUE(msg);
                    break;
                case AUDIO_PARAM_VAR2:
                    effect_param.var2 = MSG_VALUE(msg);
                    break;
                case AUDIO_PARAM_VAR3:
                    effect_param.var3 = MSG_VALUE(msg);
                    break;
                default:
                    break;
            }
        }

        // DMAバッファ転送の終了を待つ。
        wai_sem(SEM_I2SDMA);

        // プログラムが使用してもよいバッファのアドレスを取得する。
        txbuf = i2s_getTxBuf();
        rxbuf = i2s_getRxBuf();

        // 同期状態を示すためのテストピン信号を作成する。
        testpin_tp1_write(1);

        lm_left = 0;
        lm_right = 0;
        for (index = 0; index < AUDIOBUFSIZE; index+=2) {
            effect_func(
                    &effect_param,
                    rxbuf + (index + 0), rxbuf + (index + 1),
                    txbuf + (index + 0), txbuf + (index + 1));
            lm_left = (lm_left >> 1) + (*(txbuf + (index + 0)) >> 1);
            lm_right = (lm_right >> 1) + (*(txbuf + (index + 1)) >> 1);
        }

        // 同期状態を示すためのテストピン信号を作成する
        testpin_tp1_write(0);

#if 1
        /*
         * サンプルの頭の値だけを抜き出してディスプレイタスクに
         * オーディオレベルメータを要求する手抜きレベルメータ。
         */
        static int divcnt = 0;
        divcnt++;
        if ((divcnt % 64) == 0) {
            TSKAPI_DISPLAY_AUDIO_LEVELMETER(lm_left, lm_right);
        }
#endif
    }
}

