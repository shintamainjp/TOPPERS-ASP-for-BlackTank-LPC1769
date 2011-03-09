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
 * \ingroup TOPPERS_ASP
 * \defgroup TOPPERS_CHIP LPC17XX チップ依存部
 * \{
 */
/**
 * \file lpc17xx.h
 * \details NXP LPC17xxシリーズのハードウェア宣言
 */
#ifndef TOPPERS_CHIP_LPC17XX_H
#define TOPPERS_CHIP_LPC17XX_H

#include <sil.h>

/*
 * CORTEX-M3 CPUの依存部のインクルード
 */
#include "arm_m_gcc/arm_m.h"
#ifndef TOPPERS_MACRO_ONLY
#include "LPC17xx.h"
#endif

/**
 * \brief 割込み番号の最大値
 * \details
 * CORTEX-M3のNVIC割り込みコントローラは任意個の割り込み源を持つ。このマクロは、
 * 最大の割り込み番号の値を表す。
 */
#define TMAX_INTNO   (16 + 34)			// LPC1768の最大割り込み番号は16+32

/**
 * \brief 割込み優先度のビット幅
 * \details
 * CORTEX-M3のNVIC割り込みコントローラはSoC実装者の判断で最大8bitの割り込み優先値を
 * 持つことができる。TBITW_IPRIは、その優先値を表すフィールドの幅を示す。フィールド幅は
 * SoC設計者が決める。通常はユーザーズマニュアルなどに書いてある。
 *
 * LPC17xxでは5bitである。
 */
#define TBITW_IPRI     5					// LPC1768	の優先度は32順位5bit

/**
 * \brief 割込み優先度ビット幅中のサブ優先度のビット幅
 * \details
 * サブ優先度はAIRCRレジスタで指定できる。無指定の場合、0ビットが使われる。
 */
#define TBITW_SUBIPRI  0

/**
 *  \brief 割込みベクタ番号定義
 *  \details
 *  NXP提供のCMSIS LPC17xx.hで宣言されているXXXX_IRQnに以下のスクリプトを適応して生成した。irqsは、宣言を格納したファイル名である。
 *  \code
 *  sed -e "s/  //" < irqs | sed -e "s/_.*$//" | awk '{print("#define IRQ_VECTOR_"$1"\t\t(16+"$1"_IRQn)")}' -
 *  \endcode
 *  XXX_IRQnはCMSISの定義により、NVICの割り込み番号である。これはCORTEX-M3の例外番号から16を引いたものである。TOPPERS/ASPのCORTEX-M3実装では
 *  IRQ_VECTOR_XXXXを例外番号として宣言している。つまりXXX_IRQnに16を足す。
 */
#define IRQ_VECTOR_WDT		(16+WDT_IRQn)
#define IRQ_VECTOR_TIMER0		(16+TIMER0_IRQn)
#define IRQ_VECTOR_TIMER1		(16+TIMER1_IRQn)
#define IRQ_VECTOR_TIMER2		(16+TIMER2_IRQn)
#define IRQ_VECTOR_TIMER3		(16+TIMER3_IRQn)
#define IRQ_VECTOR_UART0		(16+UART0_IRQn)
#define IRQ_VECTOR_UART1		(16+UART1_IRQn)
#define IRQ_VECTOR_UART2		(16+UART2_IRQn)
#define IRQ_VECTOR_UART3		(16+UART3_IRQn)
#define IRQ_VECTOR_PWM1		(16+PWM1_IRQn)
#define IRQ_VECTOR_I2C0		(16+I2C0_IRQn)
#define IRQ_VECTOR_I2C1		(16+I2C1_IRQn)
#define IRQ_VECTOR_I2C2		(16+I2C2_IRQn)
#define IRQ_VECTOR_SPI		(16+SPI_IRQn)
#define IRQ_VECTOR_SSP0		(16+SSP0_IRQn)
#define IRQ_VECTOR_SSP1		(16+SSP1_IRQn)
#define IRQ_VECTOR_PLL0		(16+PLL0_IRQn)
#define IRQ_VECTOR_RTC		(16+RTC_IRQn)
#define IRQ_VECTOR_EINT0		(16+EINT0_IRQn)
#define IRQ_VECTOR_EINT1		(16+EINT1_IRQn)
#define IRQ_VECTOR_EINT2		(16+EINT2_IRQn)
#define IRQ_VECTOR_EINT3		(16+EINT3_IRQn)
#define IRQ_VECTOR_ADC		(16+ADC_IRQn)
#define IRQ_VECTOR_BOD		(16+BOD_IRQn)
#define IRQ_VECTOR_USB		(16+USB_IRQn)
#define IRQ_VECTOR_CAN		(16+CAN_IRQn)
#define IRQ_VECTOR_DMA		(16+DMA_IRQn)
#define IRQ_VECTOR_I2S		(16+I2S_IRQn)
#define IRQ_VECTOR_ENET		(16+ENET_IRQn)
#define IRQ_VECTOR_RIT		(16+RIT_IRQn)
#define IRQ_VECTOR_MCPWM		(16+MCPWM_IRQn)
#define IRQ_VECTOR_QEI		(16+QEI_IRQn)
#define IRQ_VECTOR_PLL1		(16+PLL1_IRQn)
#define IRQ_VECTOR_USBActivity		(16+USBActivity_IRQn)
#define IRQ_VECTOR_CANActivity		(16+CANActivity_IRQn)


/**
 *  \brief 割込みハンドラ番号定義
 *  \details
 *  NXP提供のCMSIS LPC17xx.hで宣言されているXXXX_IRQnに以下のスクリプトを適応して生成した。irqsは、宣言を格納したファイル名である。
 *  \code
 *  sed -e "s/  //" < irqs | sed -e "s/_.*$//" | awk '{print("#define INHNO_"$1"\t\tIRQ_VECTOR_"$1)}' -
 *  \endcode
 *  XXX_IRQnはCMSISの定義により、NVICの割り込み番号である。これはCORTEX-M3の例外番号から16を引いたものである。TOPPERS/ASPのCORTEX-M3実装では
 *  INHNO_XXXXを例外番号として宣言している。つまりIRQ_VECTOR_XXXと同じ値である
 */

#define INHNO_WDT		IRQ_VECTOR_WDT
#define INHNO_TIMER0		IRQ_VECTOR_TIMER0
#define INHNO_TIMER1		IRQ_VECTOR_TIMER1
#define INHNO_TIMER2		IRQ_VECTOR_TIMER2
#define INHNO_TIMER3		IRQ_VECTOR_TIMER3
#define INHNO_UART0		IRQ_VECTOR_UART0
#define INHNO_UART1		IRQ_VECTOR_UART1
#define INHNO_UART2		IRQ_VECTOR_UART2
#define INHNO_UART3		IRQ_VECTOR_UART3
#define INHNO_PWM1		IRQ_VECTOR_PWM1
#define INHNO_I2C0		IRQ_VECTOR_I2C0
#define INHNO_I2C1		IRQ_VECTOR_I2C1
#define INHNO_I2C2		IRQ_VECTOR_I2C2
#define INHNO_SPI		IRQ_VECTOR_SPI
#define INHNO_SSP0		IRQ_VECTOR_SSP0
#define INHNO_SSP1		IRQ_VECTOR_SSP1
#define INHNO_PLL0		IRQ_VECTOR_PLL0
#define INHNO_RTC		IRQ_VECTOR_RTC
#define INHNO_EINT0		IRQ_VECTOR_EINT0
#define INHNO_EINT1		IRQ_VECTOR_EINT1
#define INHNO_EINT2		IRQ_VECTOR_EINT2
#define INHNO_EINT3		IRQ_VECTOR_EINT3
#define INHNO_ADC		IRQ_VECTOR_ADC
#define INHNO_BOD		IRQ_VECTOR_BOD
#define INHNO_USB		IRQ_VECTOR_USB
#define INHNO_CAN		IRQ_VECTOR_CAN
#define INHNO_DMA		IRQ_VECTOR_DMA
#define INHNO_I2S		IRQ_VECTOR_I2S
#define INHNO_ENET		IRQ_VECTOR_ENET
#define INHNO_RIT		IRQ_VECTOR_RIT
#define INHNO_MCPWM		IRQ_VECTOR_MCPWM
#define INHNO_QEI		IRQ_VECTOR_QEI
#define INHNO_PLL1		IRQ_VECTOR_PLL1
#define INHNO_USBActivity		IRQ_VECTOR_USBActivity
#define INHNO_CANActivity		IRQ_VECTOR_CANActivity

/**
 *  \brief TOPPERS/ASP割込み番号定義
 *  \details
 *  NXP提供のCMSIS LPC17xx.hで宣言されているXXXX_IRQnに以下のスクリプトを適応して生成した。irqsは、宣言を格納したファイル名である。
 *  \code
 *  sed -e "s/  //" < irqs | sed -e "s/_.*$//" | awk '{print("#define INTNO_"$1"\t\tIRQ_VECTOR_"$1)}' -
 *  \endcode
 *  XXX_IRQnはCMSISの定義により、NVICの割り込み番号である。これはCORTEX-M3の例外番号から16を引いたものである。TOPPERS/ASPのCORTEX-M3実装では
 *  INTNO_XXXXを例外番号として宣言している。つまりIRQ_VECTOR_XXXと同じ値である
 */
#define INTNO_WDT		IRQ_VECTOR_WDT
#define INTNO_TIMER0		IRQ_VECTOR_TIMER0
#define INTNO_TIMER1		IRQ_VECTOR_TIMER1
#define INTNO_TIMER2		IRQ_VECTOR_TIMER2
#define INTNO_TIMER3		IRQ_VECTOR_TIMER3
#define INTNO_UART0		IRQ_VECTOR_UART0
#define INTNO_UART1		IRQ_VECTOR_UART1
#define INTNO_UART2		IRQ_VECTOR_UART2
#define INTNO_UART3		IRQ_VECTOR_UART3
#define INTNO_PWM1		IRQ_VECTOR_PWM1
#define INTNO_I2C0		IRQ_VECTOR_I2C0
#define INTNO_I2C1		IRQ_VECTOR_I2C1
#define INTNO_I2C2		IRQ_VECTOR_I2C2
#define INTNO_SPI		IRQ_VECTOR_SPI
#define INTNO_SSP0		IRQ_VECTOR_SSP0
#define INTNO_SSP1		IRQ_VECTOR_SSP1
#define INTNO_PLL0		IRQ_VECTOR_PLL0
#define INTNO_RTC		IRQ_VECTOR_RTC
#define INTNO_EINT0		IRQ_VECTOR_EINT0
#define INTNO_EINT1		IRQ_VECTOR_EINT1
#define INTNO_EINT2		IRQ_VECTOR_EINT2
#define INTNO_EINT3		IRQ_VECTOR_EINT3
#define INTNO_ADC		IRQ_VECTOR_ADC
#define INTNO_BOD		IRQ_VECTOR_BOD
#define INTNO_USB		IRQ_VECTOR_USB
#define INTNO_CAN		IRQ_VECTOR_CAN
#define INTNO_DMA		IRQ_VECTOR_DMA
#define INTNO_I2S		IRQ_VECTOR_I2S
#define INTNO_ENET		IRQ_VECTOR_ENET
#define INTNO_RIT		IRQ_VECTOR_RIT
#define INTNO_MCPWM		IRQ_VECTOR_MCPWM
#define INTNO_QEI		IRQ_VECTOR_QEI
#define INTNO_PLL1		IRQ_VECTOR_PLL1
#define INTNO_USBActivity		IRQ_VECTOR_USBActivity
#define INTNO_CANActivity		IRQ_VECTOR_CANActivity

#ifndef TOPPERS_MACRO_ONLY

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_CHIP_LPC17XX_H */

/**
 * \}
 */
