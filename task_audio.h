/**
 * @file task_audio.h
 * @brief オーディオDMAに同期して動くタスク
 * @param exinf コンフィギュレータがタスクに引き渡す変数。
 * @details
 * @ref dma_intr_handler からのSEM_I2SDMA セマフォ・シグナルを受けて
 * しかるべきデータ・バッファを特定し、信号処理を行う。
 *
 * なお、このタスクはシステム起動時にはインアクティブである。
 * 明示的にアクティベートして始めて動作を開始する。
 */

#ifndef TASK_AUDIO_H
#define TASK_AUDIO_H

#include "i2s_subsystem.h"

/**
 * @brief タスクプライオリティ。(オーディオタスク)
 */
#define TSKPRI_AUDIO 10
#define TSKSTK_AUDIO STACK_SIZE

/**
 * @defgroup BLACKTANK_LPC1769_TASK_AUDIO_CLIENT
 * @ingroup BLACKTANK_LPC1769_TASK_AUDIO
 * @{
 */

#define AUDIO_PARAM_MODE 0 /**< @brief モードパラメータ。 */
#define AUDIO_PARAM_VAR0 1 /**< @brief VAR0パラメータ。 */
#define AUDIO_PARAM_VAR1 2 /**< @brief VAR1パラメータ。 */
#define AUDIO_PARAM_VAR2 3 /**< @brief VAR2パラメータ。 */
#define AUDIO_PARAM_VAR3 4 /**< @brief VAR3パラメータ。 */

#define AUDIO_VALUE_MODE_THROUGH 0 /**< @brief モードに対する値。 */
#define AUDIO_VALUE_MODE_VOCAL_CANCEL 1 /**< @brief モードに対する値。 */
#define AUDIO_VALUE_MODE_FIR 2 /**< @brief モードに対する値。 */
#define AUDIO_VALUE_MODE_IIR 3 /**< @brief モードに対する値。 */

/**
 * @brief オーディオパラメータを設定する。
 * @details
 * このマクロを使うとオーディオパラメータを設定することができる。
 * マクロはタスクコンテキスト内から呼び出す事ができる。
 * @param PARAM 設定対象パラメータ。
 * @param VALUE 設定対象パラメータに対する設定値。
 */
#define TSKAPI_AUDIO_PARAM(PARAM,VALUE) \
    snd_dtq(DTQ_AUDIOPARAM, \
            (intptr_t)( \
                (((PARAM) & 0x000F) << 12) \
                | (((VALUE)& 0x0FFF) << 0)))

/**
 * @}
 */

/**
 * @defgroup BLACKTANK_LPC1769_TASK_AUDIO
 * @ingroup BLACKTANK_LPC1769_TASK
 * @{
 */

void task_audio(intptr_t exinf);
void dma_intr_handler(intptr_t exinf);
struct I2S_AUDIO_DATA* get_audio_data(void);

/**
 * @}
 */

#endif

