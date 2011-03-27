/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2007 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2010 by TOPPERS/ASP for LPC project
 *              http://sourceforge.jp/projects/toppersasp4lpc/
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
 *  アを改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを再利用可能なバイナリコード（リロケータブルオブ
 *      ジェクトファイルやライブラリなど）の形で利用する場合には，利用
 *      に伴うドキュメント（利用者マニュアルなど）に，上記の著作権表示，
 *      この利用条件および下記の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを再利用不可能なバイナリコードの形または機器に組
 *      み込んだ形で利用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 */
/**
 * \defgroup TOPPERS_ASP TOPPERS/ASP
 */
/**
 * \ingroup TOPPERS_ASP
 * \defgroup TOPPERS_TARGET LPC1768 ターゲット依存部
 */
/* \{ */
/**
 * \file lpc1768_generic.h
 * \brief ターゲットハードウェアに依存する宣言を納める
 * \details
 * アプリケーション、システムサービス、カーネルなどで利用されるハードウェア
 * 関係の宣言をまとめるファイル。
 */

/**
 * NXP社LPC1768用のターゲット依存部。TOPPERS/ASPターゲット非依存部1.6に対応している。
 * 最新ソースコードは http://sourceforge.jp/projects/toppersasp4lpc/ より入手できる。
 *
 * クロックとして内蔵オシレーターを使用しているため、外部にオシレーターをつけなくても
 * 動作させることができる。そのため、ほとんどのボードで動作する。オシレーターの設定を
 * 変更する場合には、\ref target_config.cを変更するとよい。
 *
 * コンソールはシリアルポート1(UART0)を使用している。設定は 8 data bit, 1 start bit, 1 stop bit,
 * none-parity, 57600baudである。使用するポートや速度を変更する場合には、\ref target_syssvc.hの内容を変更する。
 *
 * システム・タイマーはSYSTICK割り込みを使っている。
 *
 * ビルドするには、次のコマンドを実行してコンフィギュレータをビルドする。
 * \code
 * $ cd asp/cfg
 * $ .configure
 * $ make
 * \endcode
 *
 * また、アプリケーションのビルド時には、makeあるいはmake sramを使用する。makeはROM化コードを生成し、
 * make sramはSRAM上で走るデバッグ用コードを生成する。
 */

#ifndef TOPPERS_LPC1768_GENERIC_H
#define TOPPERS_LPC1768_GENERIC_H

#include <sil.h>

/*
 *  システムクロックの定義（発振子周波数8MHz）
 */
/**
 * \brief 内蔵クロックの発信周波数
 * \details
 * 内蔵発振器のクロック周波数[Hz]。SYS_CLOCKが参照する。LPC1768の場合は4MHzである。
 */
#define INTERNAL_CLOCK		(4000000)			// 内蔵発振器は4MHz

/**
 * \brief コア・クロック周波数
 * \details
 * CORTEX-M3コアに入力する動作クロックの周波数[Hz]。
 */
#define SYS_CLOCK		(INTERNAL_CLOCK * 25)	// コアに入力するクロック 100MHz


#ifndef TOPPERS_MACRO_ONLY

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_LPC1768_GENERIC_H */
/* \} */
