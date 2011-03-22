/**
 * @file task_audio_init.h
 * @brief オーディオ初期化タスク。
 * このタスクはオーディオデバイスの初期化を実行し、
 * オーディオ処理タスクが処理を開始するのに必要な準備を実行する。
 */

#ifndef TASK_AUDIO_INIT_H
#define TASK_AUDIO_INIT_H

#define TSKPRI_AUDIO_INIT 13

void task_audio_init(intptr_t exinf);

#endif

