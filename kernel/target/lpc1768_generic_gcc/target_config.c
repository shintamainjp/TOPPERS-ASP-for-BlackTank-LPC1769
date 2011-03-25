/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2007 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2010 by TOPPERS/ASP for LPC project
 *              http://sourceforge.jp/projects/toppersasp4lpc/
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 */
/**
 * \addtogroup TOPPERS_TARGET
 * \{
 */

/**
 * \file target_config.c
 * \brief カーネル実装のターゲット依存モジュール（NXP LPC1768用）
 * \details
 * カーネル構築に必要でターゲットに依存する定義をこのファイルで行う。
 */
#include "kernel_impl.h"
#include <sil.h>
#include "lpc1768_generic.h"
#include "target_serial.h"
#include "target_syssvc.h"

/**
 * \brief UART0のTX信号にピンを割り付けるためのビットフィールド定義
 *
 */
#define PINSEL_UART0_TX 0x10

/**
 * \brief UART0のRX信号にピンを割り付けるためのビットフィールド定義
 *
 */
#define PINSEL_UART0_RX 0x40

/**
 * \brief ターゲット依存部　初期化処理
 * \details
 * sta_ker() の中から呼び出されてターゲットのハードウェアの初期化を行う。この関数の中では
 * PLLの初期化やペリフェラル・ピンの初期化などを行う。
 */
void target_initialize(void)
{
    /*
     *  チップ依存部の初期化。この呼び出しは削除してはいけない。
     */
    chip_initialize();

    /*
     * CMSISのSystemInitを使ってターゲットを初期化する。
     */
    SystemInit();

    /*
     *  I/Oポートの初期化
     */
    /* bit5:4=01, bit7:6=01, ピンをUART0に割り当てる。*/
    LPC_PINCON->PINSEL0 = PINSEL_UART0_TX | PINSEL_UART0_RX;

    /*
     *  バナー出力用のシリアル初期化
     */
    target_uart_init(SIO_PORTID);
}

/**
 * \brief ターゲット依存部　終了処理
 * \details
 * システム終了時にカーネルから呼ばれる。この関数は呼び出し元に戻らない。
 */
void target_exit(void)
{
    /* チップ依存部の終了処理 */
    chip_exit();

    while(1) {}
}

/**
 * \brief システムログの低レベル出力のための文字出力
 * \param c 出力キャラクタ
 * \details
 * コンソールに１文字出力する。チップ依存部が供給する出力関数をそのまま利用する。
 */
void target_fput_log(char_t c)
{
    chip_fput_log(c);
}

/**
 * \}
 */

