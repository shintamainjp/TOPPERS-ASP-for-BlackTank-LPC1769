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

#ifndef TOPPERS_TARGET_CONFIG_H
#define TOPPERS_TARGET_CONFIG_H

/**
 * \file target_config.h
 * \brief カーネル実装のターゲット依存部モジュール（NXP LPC1768用）
 * \details
 * カーネルのターゲット依存部のインクルードファイル．kernel_impl.hのター
 * ゲット依存部の位置付けとなす．
 */



/**
 * \brief デフォルトの非タスクコンテキスト用のスタック領域の定義
 * \details
 *  8の倍数として指定すること
 */
#define DEFAULT_ISTKSZ			(512)		/* 0.5kB */

/**
 * \brief 微少時間待ちのための定義（本来はSILのターゲット依存部）
 * \details
 * 値はtest_dly_nseアプリケーションを使って、SYSCLK=100MHz,
 * SRAM実行時に更正した。
 */
#define SIL_DLY_TIM1    79

/**
 * \brief 微少時間待ちのための定義（本来はSILのターゲット依存部）
 * \details
 * 値はtest_dly_nseアプリケーションを使って、SYSCLK=100MHz,
 * SRAM実行時に更正した。
 */
#define SIL_DLY_TIM2    50

#ifndef TOPPERS_MACRO_ONLY

extern void target_exit(void);
extern void target_initialize(void);

#endif /* TOPPERS_MACRO_ONLY */

/*
 * チップ依存部の読み込み
 */
#include "chip_config.h"

#endif /* TOPPERS_TARGET_CONFIG_H */

/**
 * \}
 */
