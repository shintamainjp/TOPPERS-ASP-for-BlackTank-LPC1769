/**
 * @file sys_init.c
 * @author Shinichiro Nakamura
 * @brief システムで優先的に初期化したい処理の実装。
 */

#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

#include "kernel_cfg.h"
#include "sys_init.h"

/**
 * @brief 初期化ルーチン
 * @param exinf コンフィギュレータから渡す引数。今回は利用しない
 * @details
 * この関数はapplication.cfgの中で、ATT_INIによりシステムに
 * 初期化関数として登録される。
 */
void sys_init(intptr_t exinf)
{
    /*
     * システムで早期に初期化が必要なものに関してここに記述する。
     */
}

