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
 * \defgroup TOPPERS_SIO SIOドライバ
 * \ingroup TOPPERS_ASP
 */
/* \{ */
/**
 * \file uart.h
 * \brief シリアルI/Oデバイス（SIO）ドライバ（UART汎用）
 */

#ifndef TOPPERS_UART_H
#define TOPPERS_UART_H

#include "kernel.h"

/*
 *  SIOの割込みベクタ番号
 */
#if (SIO_PORTID == 1)
#define INHNO_SIO		INHNO_UART0
#define INTNO_SIO		INHNO_UART0
#elif (SIO_PORTID == 2)
#define INHNO_SIO		INHNO_UART1
#define INTNO_SIO		INHNO_UART1
#elif (SIO_PORTID == 3)
#define INHNO_SIO		INHNO_UART2
#define INTNO_SIO		INHNO_UART2
#elif (SIO_PORTID == 4)
#define INHNO_SIO		INHNO_UART3
#define INTNO_SIO		INHNO_UART3
#endif

#define INTPRI_SIO		-3				/* 割込み優先度 */
#define INTATR_SIO		0				/* 割込み属性 */

/**
 * \ingroup SIOAPI
 * \brief コールバックルーチンの識別番号
 * \details
 * porting.txt で要請されている識別番号の宣言。 SIO_RDY_RCVと区別できる
 * 番号なら何でもいい。
 */
#define SIO_RDY_SND    1U        /* 送信可能コールバック */
/**
 * \ingroup SIOAPI
 * \brief コールバックルーチンの識別番号
 * \details
 * porting.txt で要請されている識別番号の宣言。 SIO_RDY_SNDと区別できる
 * 番号なら何でもいい。
 */
#define SIO_RDY_RCV    2U        /* 受信通知コールバック */

#ifndef TOPPERS_MACRO_ONLY

/**
 * \ingroup SIOAPI
 * \brief シリアルポートの管理ブロック
 * \details
 * porting.txtの8.3節で要請されている管理ブロックデータ型。
 * シリアル・サービス・タスクとSIOペリフェラルAPIの間での管理データの
 * やりとりに使われる。
 */
typedef struct sio_port_control_block {
	ID peripheral_index;	/**< ペリフェラル番号。0オリジン */
	uint32_t reg_base;	/**< SIOペリフェラルのレジスタ・ベース・アドレス */
	intptr_t exinf; 		/**< sio_opn_porが渡すexinf引数の格納場所。 */
} SIOPCB;

/*
 *  SIO初期化
 */
extern void sio_initialize(intptr_t exinf);

/*
 *  シリアルオープン
 */
extern SIOPCB *sio_opn_por(ID siopid, intptr_t exinf);

/*
 *  シリアルクローズ
 */
extern void sio_cls_por(SIOPCB *p_siopcb);

/*
 *  割込みハンドラ
 */
extern void sio_isr(intptr_t exinf);

/*
 *  1文字送信
 */
extern bool_t sio_snd_chr(SIOPCB *siopcb, char_t c);

/*
 *  1文字受信
 */
extern int_t sio_rcv_chr(SIOPCB *siopcb);

/*
 *  コールバックの許可
 */
extern void sio_ena_cbr(SIOPCB *siopcb, uint_t cbrtn);

/* 
 *  コールバックの禁止
 */
extern void sio_dis_cbr(SIOPCB *siopcb, uint_t cbrtn);

/*
 *  送信可能コールバック
 */
extern void sio_irdy_snd(intptr_t exinf);

/*
 *  受信通知コールバック
 */
extern void sio_irdy_rcv(intptr_t exinf);

/*
 *  1文字送信（ポーリングでの出力）
 */
extern void sio_pol_snd_chr(char_t c, ID siopid);

/*
 *  ターゲットのシリアル初期化
 */
extern void target_uart_init(ID siopid);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_TARGET_SERIAL_H */
/* \} */
