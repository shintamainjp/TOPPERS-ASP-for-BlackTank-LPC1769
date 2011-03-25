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

#ifndef TOPPERS_CHIP_CONFIG_H
#define TOPPERS_CHIP_CONFIG_H
/**
 * \addtogroup TOPPERS_CHIP
 * \{
 */

/**
 * \file chip_config.h
 * \brief カーネル実装のチップ依存部モジュール（NXP LPC17XX用汎用）
 * \details
 * カーネルのターゲット依存部のインクルードファイル．kernel_impl.hのター
 * ゲット依存部の位置付けとなす．
 */

/*
 * チップ依存部の読み込み
 */
#include "lpc17xx.h"

/*
 *  トレースログに関する設定
 */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_ENABLE_TRACE */




/**
 * \brief 非タスクコンテキスト用スタックポインタの初期値を格納する変数
 * \details
 * porting.txtは、*_kernel_istkptを変数として使い、そこに非タスクコンテキスト用
 * スタックポインタの初期値を格納することを要請している。LPC1763の場合、*_kernel_istkptの
 * 値は、SRAM実行とROM実行で異なる。これに対応するため、LDスクリプトで例外ベクトルの先頭を
 * 指す値、_vector_start_lmaを作り、_kernel_istkptとして使用している。
 */
#define _kernel_istkpt _vector_start_lma

#ifndef DEFAULT_ISTKSZ
/**
 * \brief デフォルトの非タスクコンテキスト用のスタック領域の定義
 * \details
 *  割り込み用のスタックのサイズを指定する。8の倍数として指定すること。このマクロは
 *  ターゲット依存部で指定していない場合にのみ使用される。
 */
#define DEFAULT_ISTKSZ			(2048)
#endif

#ifndef TOPPERS_MACRO_ONLY

/**
 *  \brief チップ依存の初期化
 *  \details
 *  この館数はtarget_initializeから呼ぶことを前提としている。
 */
extern void chip_initialize(void);

/**
 * \brief チップの終了処理
 * \details
 *  システムを終了する時に使う．この関数はtarget_exit()から
 *  呼ぶことを前提としており、prc_terminate()を呼び、処理が終了すると制御を戻す。
 */
extern void chip_exit(void);

/**
 * \brief システムログの低レベル出力のための文字出力
 * \param c 出力キャラクタ
 * \details
 * コンソールに１文字出力する。
 */
extern void chip_fput_log(char_t c);


#endif /* TOPPERS_MACRO_ONLY */

/*
 *  プロセッサ依存モジュール（ARM-M用）
 */
#include "arm_m_gcc/prc_config.h"

#endif /* TOPPERS_CHIP_CONFIG_H */

/**
 * \}
 */

