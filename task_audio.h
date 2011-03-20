
#ifndef TASK_AUDIO_H
#define TASK_AUDIO_H

#define TSKPRI_AUDIO 10

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
void task_audio(intptr_t exinf);
void dma_intr_handler(intptr_t exinf);

#endif

