/**
 * \file app_init.c
 * \brief アプリケーションに関連する初期化用ファイル。
 */

#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

#include "kernel_cfg.h"
#include "app_init.h"

/**
 * \brief 初期化ルーチン
 * \param exinf コンフィギュレータから渡す引数。今回は利用しない
 * \details
 *
 * この関数はapplication.cfgのなかで、ATT_INIにより、
 * システムに初期化関数として登録される。
 */
void app_init(intptr_t exinf)
{
}

