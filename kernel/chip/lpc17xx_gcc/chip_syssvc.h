/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
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
 * \file chip_syssvc.h
 * \brief システムサービスのチップ依存部（NXP LPC17xx用）
 * \details
 *  システムサービスのターゲット依存部のインクルードファイル．このファ
 *  イルの内容は，コンポーネント記述ファイルに記述され，このファイルは
 *  無くなる見込み．
 */

#ifndef TOPPERS_CHIP_SYSSVC_H
#define TOPPERS_CHIP_SYSSVC_H

/*
 *  ターゲットシステムのハードウェア資源の定義
 */
#include "lpc17xx.h"

/*
 *  トレースログに関する設定
 */
#ifdef TOPPERS_TRACE_ENABLE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_TRACE_ENABLE */


/**
 * \brief 起動メッセージ
 * \details
 *  起動メッセージにチップ依存部の著作権表示を
 *  追加するためのマクロ．
 */
#ifdef PRC_COPYRIGHT
#define CHIP_COPYRIGHT	PRC_COPYRIGHT "Copyright (C) 2010 by TOPPERS/ASP for LPC project\n            http://sourceforge.jp/projects/toppersasp4lpc/\n"
#else
#define CHIP_COPYRIGHT	  "Copyright (C) 2010 by TOPPERS/ASP for LPC project\n            http://sourceforge.jp/projects/toppersasp4lpc/\n"
#endif /* CHIP_COPYRIGHT */


/**
 * \brief UARTへの入力クロック
 * \details
 * 内蔵UARTのクロック周波数を指定する。単位はHz。LPC17xxはそれぞれのUARTに対して
 * 独立した周波数を入力できるが、今回の実装ではtarget_serial.cはすべてのUARTに
 * 同じクロックが入力されれると仮定している。
 *
 * この実装では、UART用のペリフェラル・クロックの分周比は1/4であり、これはリセット値である。
 */
#define SIO_UART_CLOCK	(SYS_CLOCK/4)

#ifndef TOPPERS_MACRO_ONLY

/**
 * \brief システムログの低レベル出力のための文字出力
 * \details
 *  ターゲット依存の方法で，文字cを表示/出力/保存する．LPC17XXの実装では、
 *  pdic/uartのポーリングによる出力ルーチンを呼ぶ。
 *
 *  このルーチンを呼ぶ前に、初期化として #target_uart_init()を一度呼ばなければ
 *  ならない。ターゲット依存部の#target_initialize()で呼ぶとよい。
 */
extern void	chip_fput_log(char_t c);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_CHIP_SYSSVC_H */

/**
 * \}
 */

