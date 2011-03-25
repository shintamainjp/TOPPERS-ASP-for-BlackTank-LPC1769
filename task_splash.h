/**
 * @file task_splash.h
 * @brief オーディオ初期化タスク。
 * このタスクはオーディオデバイスの初期化を実行し、
 * オーディオ処理タスクが処理を開始するのに必要な準備を実行する。
 */

#ifndef TASK_SPLASH_H
#define TASK_SPLASH_H

#define TSKPRI_SPLASH 13
#define TSKSTK_SPLASH STACK_SIZE

void task_splash(intptr_t exinf);

#endif

