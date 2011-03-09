/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
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
 * \file chip_stddef.h
 * \brief t_stddef.hのチップ依存部（NXP LPC17xx用）
 * \details
 *  このインクルードファイルは，target_stddef.hの先頭でインクルードされる．
 *  他のファイルからは直接インクルードすることはない．他のインクルード
 *  ファイルに先立って処理されるため，他のインクルードファイルに依存し
 *  てはならない．
 */

#ifndef TOPPERS_CHIP_STDDEF_H
#define TOPPERS_CHIP_STDDEF_H

/**
 * \brief ターゲットを識別するためのマクロの定義
 * \details
 * 各種のチップに対応可能なコードの中で、LPC17xxに依存する部分がある場合は
 * このマクロによる条件コンパイルにする。
 *
 * ターゲット依存部とはっきり区別するため、チップ依存部の識別名の前にTOPPERS_CHIP_を
 * つけることを推奨する。
 */
#define TOPPERS_CHIP_LPC17XX			/* チップ略称 */


/*
 *  開発環境で共通な定義
 */
#ifndef TOPPERS_MACRO_ONLY
#include "stdint.h"
#endif

/**
 * \brief 標準浮動小数点型を使う
 * \details
 * IEEE標準の浮動小数点型を使うことを宣言。version.txt参照
 */
#define TOPPERS_STDFLOAT_TYPE1
#include "gcc/tool_stddef.h"

/*
 *  プロセッサで共通な定義
 */
#include "arm_m_gcc/prc_stddef.h"

#endif /* TOPPERS_CHIP_STDDEF_H */

/**
 * \}
 */
