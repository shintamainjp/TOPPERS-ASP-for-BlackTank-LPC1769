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
 * \addtogroup TOPPERS_CHIP
 * \{
 */
/**
 * \file chip_config.c
 * \brief カーネル実装のチップ依存モジュール（LPC17xx汎用）
 */
#include "kernel_impl.h"
#include <sil.h>
#include "lpc17xx.h"
#include "target_serial.h"
#include "target_syssvc.h"

void chip_initialize(void)
{
    /*
     *  プロセッサ依存部の初期化
     */
    prc_initialize();
}

void chip_exit(void)
{
    /* チップ依存部の終了処理 */
    prc_terminate();
}

void chip_fput_log(char_t c)
{
    /*
     * Newlineなら、CRも追加する
     */
    if (c == '\n') {
        sio_pol_snd_chr('\r', SIO_PORTID);
    }
    sio_pol_snd_chr(c, SIO_PORTID);
}

/**
 * \brief 低レベルのターゲット依存の初期化
 * \details
 * メモリ初期化の前に呼び出される
 * CCRのSTKALIGNビットをクリアする
 * このビットはCORTEX-M3コアがR1からR2に変化する過程で"1"に変更された。
 * クリアしないとTOPPERS/ASPはクラッシュする。
 */
void hardware_init_hook()
{
    SCB->CCR &= ~SCB_CCR_STKALIGN_Msk;
}

/**
 * \}
 */

