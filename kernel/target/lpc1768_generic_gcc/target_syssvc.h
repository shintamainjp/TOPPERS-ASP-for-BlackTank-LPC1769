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
 * \addtogroup TOPPERS_TARGET
 * \{
 */

/**
 * \file target_syssvc.h
 * \brief システムサービスのターゲット依存部（LPC1768用）
 * \details
 * porting.txt８章で示される、システムサービスのターゲット依存部。
 *
 *  システムサービスのターゲット依存部のインクルードファイル．このファ
 *  イルの内容は，コンポーネント記述ファイルに記述され，このファイルは
 *  無くなる見込み．
 */

#ifndef TOPPERS_TARGET_SYSSVC_H
#define TOPPERS_TARGET_SYSSVC_H


/*
 *  ターゲットシステムのハードウェア資源の定義
 */
#include "lpc1768_generic.h"
/*
 *  チップ共有のハードウェア資源の読み込み
 */
#include "chip_syssvc.h"


/**
 * \brief 起動メッセージのターゲットシステム名
 */
#define TARGET_NAME    "LPC1768 Generic"

/*
 *  起動メッセージにターゲット依存部の著作権表示を
 *  追加する必要があるときには、このマクロを使う。
 */
#if 0
#define TARGET_COPYRIGHT	CHIP_COPYRIGHT "Copyright (C) 2010 by TOPPERS/ASP for LPC project\n            http://sourceforge.jp/projects/toppersasp4lpc/"
#else
#define TARGET_COPYRIGHT CHIP_COPYRIGHT
#endif

/**
 * \brief コンソールが使用するシリアルポートID
 * \details
 * UIF用コンソールに使用するポート番号である。ポート番号は１から始まる整数。
 */
#define SIO_PORTID		(1)

/**
 * \brief UARTドライバが管理する内蔵UARTの数
 */
#define TNUM_SIOP_UART   (1)

/**
 * \brief システムが管理するシリアル・ポートの総数
 * \details
 * この実装では内蔵UARTしか使用しないので、TNUM_PORT = TNUM_SIOP_UARTになる
 */
#define TNUM_PORT        TNUM_SIOP_UART		/* システムが管理するシリアルポートの総数 */

/**
 * \brief システムログ用のシリアルポート番号
 * \details
 * logtaskで使用する。1オリジン。
 *
 * システムログ用のシリアルポートは、コンソール用のシリアルポートと強要することが多い。
 */
#define LOGTASK_PORTID   SIO_PORTID

/**
 * \brief シリアルポートのデフォルトボーレート
 * \details
 * TOPPERS/ASPのシリアルポート1に対するボーレート設定値である。
 * シリアルポートの番号は1オリジンになっている。
 * 特に設定しない限り、シリアルポートにはデフォルトの値が設定される。
 * シリアルポートの速度を変えたければ、マクロSIO_BAUD_RATE_PORTxを宣言する。
 */
#define SIO_BAUD_RATE_DEFAULT		(57600)

//#define SIO_BAUD_RATE_PORT1		(57600)
//#define SIO_BAUD_RATE_PORT2		(57600)
//#define SIO_BAUD_RATE_PORT3		(57600)
//#define SIO_BAUD_RATE_PORT4		(57600)

/**
 * \brief システムログの低レベル出力のための文字出力
 * \details
 *  ターゲット依存の方法で，文字cを表示/出力/保存する．
 */
extern void	target_fput_log(char_t c);

#endif /* TOPPERS_TARGET_SYSSVC_H */
/**
 * \}
 */
