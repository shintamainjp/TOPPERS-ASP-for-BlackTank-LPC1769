/**
 * \file app_init.c
 * \brief アプリケーションに関連する初期化用ファイル。
 */

#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

#include "kernel_cfg.h"
#include "app_init.h"
#include "hardware.h"

/**
 * \brief 初期化ルーチン
 * \param exinf コンフィギュレータから渡す引数。今回は利用しない
 * \details
 *
 * LED出力ポートの初期化を行う。LPCXpresso 1768のLEDは、ピンP0.22に
 * 接続されている。そこで、ITRONのイニシャライザ機能を使ってピンを初期化する。
 * この関数はapplication.cfgのなかで、ATT_INIにより、システムに初期化関数として
 * 登録される。
 */
void app_init(intptr_t exinf)
{
    /*
     *  I/Oポートの初期化
     * LEDはP0.22。1のときオン。
     */

    /*
     * まず出力状態を「消灯」にしておく
     */
    LPC_GPIO0->FIOCLR = ACTLED;

    /*
     * P0.22を出力にする
     */
    LPC_GPIO0->FIODIR |= ACTLED;
}

