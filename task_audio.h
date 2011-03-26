/**
 * @file task_audio.h
 * @brief オーディオDMAに同期して動くタスク
 * @param exinf コンフィギュレータがタスクに引き渡す変数。
 * @details
 * @ref dma_intr_handler からのSEM_I2SDMA セマフォ・シグナルを受けて
 * しかるべきデータ・バッファを特定し、信号処理を行う。
 *
 * なお、このタスクはシステム起動時にはインアクティブである。
 * @ref main_task から明示的にアクティベートして始めて動作を開始する。
 */

#ifndef TASK_AUDIO_H
#define TASK_AUDIO_H

#include "i2s_subsystem.h"

#define TSKPRI_AUDIO 10
#define TSKSTK_AUDIO 2048

#define AUDIO_PARAM_VAR0 0
#define AUDIO_PARAM_VAR1 1
#define AUDIO_PARAM_VAR2 2
#define AUDIO_PARAM_VAR3 3

#define AUDIO_PARAM(TARGET,VALUE) \
    snd_dtq(DTQ_AUDIOPARAM, \
            (intptr_t)( \
                (((TARGET) & 0x000F) << 12) \
                | (((VALUE)& 0x0FFF) << 0)))

void task_audio(intptr_t exinf);
void dma_intr_handler(intptr_t exinf);
struct I2S_AUDIO_DATA* get_audio_data(void);

#endif

