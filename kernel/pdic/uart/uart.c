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
 * \file uart.c
 * \ingroup TOPPERS_SIO
 * \brief ターゲット依存のシリアル・ペリフェラル制御ルーチン群
 * \details
 *  ターゲット依存のシリアル・ペリフェラル制御ルーチン群。TOPPERS/ASPのシリアル
 *  ・タスクの下請けとして働く。詳細に関してはporting.txtを参照のこと。
 *
 */

/*
 * シリアルドライバ（UART汎用）
 */

#include <kernel.h>
#include <sil.h>
#include "uart.h"
#include "target_syssvc.h"

/*
 * レジスタ設定値
 */

/**
 * \brief SIOPCBのインデックスからポートID番号への変換
 * \details
 * TOPPERS/ASPのSIOポートID番号は1オリジンだが、 siopcb_table[]は
 * 0オリジンである。そのため、変換関数を使って与えられたインデックス番号から
 * ポートID番号を作り出す。
 *
 */
#define INDEX2SIOPID(x)	((x) + 1)

/**
 * \brief ポート番号からSIOPCBのインデックスへの変換
 * \details
 * TOPPERS/ASPのSIOポートID番号は1オリジンだが、 siopcb_table[]は
 * 0オリジンである。そのため、変換関数を使って与えられたポートID番号から
 * PCBを取得するためのインデックスを作り出す。
 */
#define SIOPID2INDEX(x)	((x) - 1)

/**
 * \brief ポートID番号からSIOPCBを取得する
 * \details
 * ポートID番号を元に、SIOPCBへのポインタを取得する。
 */
#define GET_SIOPCB(x)	(&siopcb_table[SIOPID2INDEX(x)])

/**
 * \brief ペリフェラル・レジスタの間隔
 * \details
 * ペリフェラルレジスタが何バイトごとに現れるかを記述する。この
 * 記述はチップごとに変わる。
 */
#ifdef TOPPERS_CHIP_LPC17XX
#define UART_BOUNDARY 4
#else
#error "You must define UART_BOUNDARY for your architecture"
#endif

/**
 * \brief 8250, 16450, 16550各レジスタのアドレス・オフセット。
 * \details
 * UART_RHRを基点とする。
 * システムごとに異なるのでUART_BOUNDARYによってレジスタの配置境界を指定する。
 * たとえばすべてのレジスタが8ビット境界で配置されているなら、UART_BOUNDARYは1,
 * 16bit境界なら2, 32ビット境界なら4を指定する。
 * UART_BOUNDARYはsys_config.hで定義する。
 */

#define UART_RBR   (0x00 * UART_BOUNDARY)   /*  受信バッファレジスタ  */
#define UART_THR   (0x00 * UART_BOUNDARY)   /*  送信データ・ホールド・レジスタ */
#define UART_DLL   (0x00 * UART_BOUNDARY)   /*  分周レジスタ下位バイト       */
#define UART_IER   (0x01 * UART_BOUNDARY)   /*  割込みイネーブルレジスタ       */
#define UART_DLM   (0x01 * UART_BOUNDARY)   /*  分周レジスタ上位バイト       */
#define UART_IIR   (0x02 * UART_BOUNDARY)   /*  割込み識別レジスタ  */
#define UART_FCR   (0x02 * UART_BOUNDARY)   /*  FIFO制御レジスタ  */
#define UART_LCR   (0x03 * UART_BOUNDARY)   /*  ライン制御レジスタ */
#define UART_MCR   (0x04 * UART_BOUNDARY)   /*  モデム制御レジスタ */
#define UART_LSR   (0x05 * UART_BOUNDARY)   /*  ライン・ステータス・レジスタ */
#define UART_MSR   (0x06 * UART_BOUNDARY)   /*  モデム・ステータス・レジスタ */
#define UART_SCR   (0x07 * UART_BOUNDARY)   /*  スクラッチ・パッド・レジスタ */
#define UART_GCTL  (0x09 * UART_BOUNDARY)	  /*  Blackfinのみ。グローバル制御レジスタ */

#define ISR_TX            0x02       /* 送信割り込み発生 */
#define IER_TX            0x02       /* 送信割り込み許可 */
#define ISR_RX            0x01       /* 受信割り込み発生 */
#define IER_RX            0x01       /* 受信割り込み許可 */

#define LCR_DLAB          0x80       /* Divisor Enable */
#define LCR_NP_8_1        0x03       /* 8bit,1stop,Noparity,No break */
#define FCR_FIFO_DISABLE  0x00

#define LSR_RX_DATA_READY 0x01
#define LSR_TX_EMPTY      0x20

#define MCR_INT_ENABLE    0x08

/* ディバイザ計算マクロ */
#define DLM(divisor) (divisor/256)
#define DLL(divisor) (divisor%256)


/**
 * \brief シリアルI/Oポート管理ブロックエリア
 */
SIOPCB siopcb_table[TNUM_PORT];

/**
 * \brief 各シリアル・ペリフェラルのレジスタのベースアドレス。
 * \details
 * この変数は初期化関数 sio_initialize()の中で参照され、
 * レジスタアドレスは siopcb_table[]のしかるべきメンバー変数に
 * 代入される。
 */

#ifdef TOPPERS_CHIP_LPC17XX
static const uint32_t sioreg_table[TNUM_PORT] = {
	LPC_UART0_BASE
#if (TNUM_PORT >= 2)
	,LPC_UART1_BASE
#endif
#if (TNUM_PORT >= 3)
	,LPC_UART2_BASE
#endif
#if (TNUM_PORT >= 4)
	,LPC_UART3_BASE
#endif
};
#else
#error "You must define sioreg_table[]"
#endif

/**
 * \ingroup SIOSUPPORT
 * \brief シリアルポート・ペリフェラル書き込み関数
 * \param addr UARTペリフェラルのベースアドレス。通常はTHRのアドレスである。
 * \param offset ベースアドレスに対する、オフセット値。単位はバイト。
 * \param val レジスタに書き込む値。
 * \details
 * ベースアドレスとオフセットを整数で受け取り、ペリフェラルレジスタに
 * アクセスする。
 */
Inline void
uart_write(uint32_t addr, uint32_t offset, uint8_t val)
{
#ifdef TOPPERS_CHIP_LPC17XX
    sil_wrb_mem((void *)(addr + offset),val);
#else
#error "You must define access method for your architecture"
#endif
}

/**
 * \ingroup SIOSUPPORT
 * \brief シリアルポート・ペリフェラル読み込み関数
 * \param addr UARTペリフェラルのベースアドレス。通常はTHRのアドレスである。
 * \param offset ベースアドレスに対する、オフセット値。単位はバイト。
 * \return レジスタから読み込んだ値。
 * \details
 * ベースアドレスとオフセットを整数で受け取り、ペリフェラルレジスタに
 * アクセスする。
 */
Inline uint8_t
uart_read(uint32_t addr, uint32_t offset)
{
#ifdef TOPPERS_CHIP_LPC17XX
    return(sil_reb_mem((void *)(addr + offset)));
#else
#error "You must define access method for your architecture"
#endif
}

/**
 * \ingroup TOPPERS_SIO
 * \defgroup SIOAPI シリアル・サービス・タスク用API
 * \details
 * シリアル・サービス・タスクから呼び出される関数群。シリアル・サービス・タスクはこれらの
 * 関数を使ってペリフェラルを制御する。詳細はporting.txtの8.3節を参照。
 * \{
 */


/**
 * \brief シリアル・ポートが送信可能か調べる
 * \param siopcb SIOを指し示すポート・コントロール・ブロックへのポインタ
 * \return 送信可能であれば真、不能であれば偽を返す
 * \details
 * siopcbが指し示すシリアル・ペリフェラルを調べ、送信可能なら真を返す。
 *
 * 送信可能か否かの判断には、送信割り込みが許可されているかどうかも含まれる。イネーブルかつ送信レジスタに空きがあるなら
 * 送信可能である。送信の可否に割り込み許可状態を含めるのは、送信割り込み禁止状態で送信コールバックを呼ぶことを防ぐため
 * である。
 *
 * sio_putready()関数は sio_isr()関数から送信可能かどうかをチェックするために呼ばれる。もし、可能であれば sio_isr()は
 * シリアル制御サービスの sio_irdy_snd()コールバック関数を呼び出してしまう。コールバック禁止状態でこの関数を呼ばない
 * ようにするには、割り込み許可状態の確認が不可欠である。割り込み許可状態の設定に関しては sio_ena_cbr()関数と
 * sio_dis_cbr()関数を参照のこと。
 */
Inline bool_t  sio_putready(SIOPCB* siopcb)
{

	uint32_t reg = siopcb->reg_base;	/* UARTのベースアドレスを取得 */

		/* THRが空ならば送信可能なので真を返す */
	return (uart_read( reg, UART_LSR ) & LSR_TX_EMPTY);
}

/**
 * \brief シリアル・ポートに受信データがあるか調べる
 * \param siopcb SIOを指し示すポート・コントロール・ブロックへのポインタ
 * \return 送信受信データがあれば真、なければ偽を返す
 * \details
 * siopcbが指し示すシリアル・ペリフェラルを調べ、受信データがあるなら真を返す。
 *
 * 受信可能か否かの判断には、受信割り込みが許可されているかどうかも含まれる。イネーブルかつ受信レジスタに受信データがあるなら
 * 受信可能である。受信の可否に割り込み許可状態を含めるのは、受信割り込み禁止状態で受信コールバックを呼ぶことを防ぐため
 * である。
 *
 * sio_getready()関数は sio_isr() 関数から受信可能かどうかをチェックするために呼ばれる。もし、可能であれば sio_isr()は
 * シリアル制御サービスの sio_irdy_rcv()コールバック関数を呼び出してしまう。コールバック禁止状態でこの関数を呼ばない
 * ようにするには、割り込み許可状態の確認が不可欠である。割り込み許可状態の設定に関しては sio_ena_cbr()関数と
 * sio_dis_cbr()関数を参照のこと。
 */
Inline bool_t sio_getready(SIOPCB* siopcb)
{
	uint32_t reg = siopcb->reg_base;	/* UARTのベースアドレスを取得 */

		/* RBRがいっぱいならば受信可能なので真を返す */
	return (uart_read( reg, UART_LSR ) & LSR_RX_DATA_READY);
}

/**
 * \}
 */

/**
 * \ingroup TOPPERS_SIO
 *  \defgroup SIOSUPPORT SIOサポート関数群
 *  \{
 */

/**
 *  \brief ターゲットのシリアル初期化
 *  \param siopid SIOのポートID
 *  \details
 *  siopidで指定されるターゲットのSIOペリフェラルを初期化する。初期化するのは、ストップビットやビット数といった
 *  プロトコルのほか、ボーレートも含む。この館数は、target_initialize() 関数からも呼び出され、バナー出力用の
 *  ポート初期化に使用される。
 *
 *  ボーレートの設定にはUARTへ供給されるクロック周波数の設定が必要であるが、これは全UARTで共通であると
 *  仮定して決めうちしている。周波数はSIO_UART_CLOCKで指定する。単位はHzである。また、ボーレートは
 *
 *  SIO_BAUD_RATE_DEFAULTに設定されるが、上書きしたければSIOx_BAUD_RATEを宣言する。単位はBAUDである。
 */
void target_uart_init(ID siopid)
{
	uint32_t reg = sioreg_table[SIOPID2INDEX(siopid)];		/* 使用するUARTのベースアドレスを取得 */

	/* UARTの無効化 */
	uart_write( reg, UART_LCR, 0 );				/* DLAB モードをいったんクリアする */
	uart_write( reg, UART_FCR, 0 );				/* FIFO クリア*/
	uart_write( reg, UART_IER, 0 );				/* 割り込みをディセーブルにする */

	/* ボーレートの設定 */
	uart_write( reg, UART_LCR, LCR_DLAB );	/* ディバイザ設定モードに移行 */

        switch (siopid) {
            case 1:
#if defined(SIO_BAUD_RATE_PORT1)
                uart_write( reg, UART_DLL, DLL((SIO_UART_CLOCK/SIO_BAUD_RATE_PORT1/16)) );
                uart_write( reg, UART_DLM, DLM((SIO_UART_CLOCK/SIO_BAUD_RATE_PORT1/16)) );
#else
                uart_write( reg, UART_DLL, DLL((SIO_UART_CLOCK/SIO_BAUD_RATE_DEFAULT/16)) );
                uart_write( reg, UART_DLM, DLM((SIO_UART_CLOCK/SIO_BAUD_RATE_DEFAULT/16)) );
#endif
                break;
            case 2:
#if defined(SIO_BAUD_RATE_PORT2)
                uart_write( reg, UART_DLL, DLL((SIO_UART_CLOCK/SIO_BAUD_RATE_PORT2/16)) );
                uart_write( reg, UART_DLM, DLM((SIO_UART_CLOCK/SIO_BAUD_RATE_PORT2/16)) );
#else
                uart_write( reg, UART_DLL, DLL((SIO_UART_CLOCK/SIO_BAUD_RATE_DEFAULT/16)) );
                uart_write( reg, UART_DLM, DLM((SIO_UART_CLOCK/SIO_BAUD_RATE_DEFAULT/16)) );
#endif
                break;
            case 3:
#if defined(SIO_BAUD_RATE_PORT3)
                uart_write( reg, UART_DLL, DLL((SIO_UART_CLOCK/SIO_BAUD_RATE_PORT3/16)) );
                uart_write( reg, UART_DLM, DLM((SIO_UART_CLOCK/SIO_BAUD_RATE_PORT3/16)) );
#else
                uart_write( reg, UART_DLL, DLL((SIO_UART_CLOCK/SIO_BAUD_RATE_DEFAULT/16)) );
                uart_write( reg, UART_DLM, DLM((SIO_UART_CLOCK/SIO_BAUD_RATE_DEFAULT/16)) );
#endif
                break;
            case 4:
#if defined(SIO_BAUD_RATE_PORT4)
                uart_write( reg, UART_DLL, DLL((SIO_UART_CLOCK/SIO_BAUD_RATE_PORT4/16)) );
                uart_write( reg, UART_DLM, DLM((SIO_UART_CLOCK/SIO_BAUD_RATE_PORT4/16)) );
#else
                uart_write( reg, UART_DLL, DLL((SIO_UART_CLOCK/SIO_BAUD_RATE_DEFAULT/16)) );
                uart_write( reg, UART_DLM, DLM((SIO_UART_CLOCK/SIO_BAUD_RATE_DEFAULT/16)) );
#endif
                break;
        }

	/* デバイザ設定モードから、通常モードに戻す */
	uart_write( reg, UART_LCR, 0 );				/* 通常モードに移行 */

	/* プロトコルを設定。8bit, 1stop bit, parityなし */
	uart_write( reg, UART_LCR, LCR_NP_8_1 );		/* 通常モードに移行 */

}

/**
 *  \brief ターゲットのシリアル終了
 *  \param siopid 終了するポートの番号
 *  \details
 *  指定したポートをディセーブルする。
 *
 */
void target_uart_term(ID siopid)
{
	uint32_t reg = sioreg_table[SIOPID2INDEX(siopid)];

	/* UARTの無効化 */
	uart_write( reg, UART_IER, 0 );				/* 割り込みをディセーブルにする */
}

/**
 * \}
 */

/**
 * \ingroup SIOAPI
 *  \brief SIO初期化
 *  \param exinf ATTINI静的APIが渡す引数
 *  \details
 *  porting.txtにはtarget_serial.cfgにこの関数を初期化ルーチンとして
 *  登録する静的APIを含めることになっている( ASP 1.3.2 )。
 *
 *  この関数はシリアル・ポート・サービス・タスク用に内部変数を初期化するだけであり、
 *  デバイスの初期化は行っていない。デバイスの初期化はsio_opn_porが呼ばれたときに毎回行われる。
 */
void sio_initialize(intptr_t exinf)
{
	int i;

	for (i = 0; i < TNUM_PORT; i++)
	{
		siopcb_table[i].peripheral_index = i;
		siopcb_table[i].reg_base = sioreg_table[i];
		siopcb_table[i].exinf = 0;
	}
}

/**
 * \ingroup SIOAPI
 * \brief シリアル・オープン
 * \param siopid SIOを識別する番号。1から TNUM_PORTまでの整数
 * \param exinf 拡張情報。コールバックを呼ぶときに引数として渡す
 * \details
 * シリアル・ペリフェラルを初期化する。ペリフェラルはプロトコル、スピードなどの
 * 設定を終え、割り込み禁止状態で待機する。この関数に続いて sio_ena_cbrが
 * 呼び出される。
 *
 * 引数extinfで渡される情報を処理する必要はない。この引数はそのまま
 * sio_irdy_sndおよび sio_irdy_rcvコールバックに渡される。
 *
 * プロトコルは8bit, 1stopbit, parityなしに決めうちしていい。
 */
SIOPCB *sio_opn_por(ID siopid, intptr_t exinf)
{
	SIOPCB* siopcb;

	if (siopid > TNUM_PORT) {
		return NULL;
	}

	siopcb = GET_SIOPCB(siopid);
	siopcb->exinf = exinf;

	target_uart_init(siopid);

	return siopcb;
}

/**
 * \ingroup SIOAPI
 * \brief シリアルクローズ
 * \param p_siopcb ポート識別用PCBへのポインタ
 * \details
 * ターゲットのポートを閉じる。ポートはディセーブルされ、割り込みは起きない。
 */
void sio_cls_por(SIOPCB *p_siopcb)
{
	target_uart_term(INDEX2SIOPID(p_siopcb->peripheral_index));
}

/**
 * \ingroup SIOAPI
 * \brief 割込みサービスルーチン
 * \param exinf 拡張
 * \details
 * porting.txtの8.3節で養成されている割り込みサービスルーチン。
 * このルーチンはextinfの情報を元にどの割り込みが発生したか自動判別
 * している。
 *
 * extinfにどのような情報を与えるかは、明言されていないように思える。
 * CORTEX-M3版ではポート番号(1から始まってTNUM_PORTまでの識別
 * 番号)がtarget_erial.cfgの中でATTISRのextinf引数として与えられている。
 */
void sio_isr(intptr_t exinf)
{
	SIOPCB* siopcb = GET_SIOPCB(exinf);
	int32_t reg = siopcb->reg_base;
		/* 送信コールバック可能でかつ送信可能なら、送信コールバックを呼ぶ  */
	if (sio_putready(siopcb) && ( uart_read(reg, UART_IER) &  IER_TX)) {
		sio_irdy_snd(siopcb->exinf);
	}
		/* 受信コールバック可能でかつ受信可能なら、受信コールバックを呼ぶ  */
	if (sio_getready(siopcb) && ( uart_read(reg, UART_IER) &  IER_RX)) {
		sio_irdy_rcv(siopcb->exinf);
	}
}

/**
 * \ingroup SIOAPI
 * \brief 1文字送信
 * \param siopcb 送信するSIOポート
 * \param c 送信する文字
 * \return 送信に成功したら真、失敗したら偽
 * \details
 * シリアル制御タスクから呼び出されて１文字送信する関数。送信できた時には真を、できなかったときには偽を返す。
 * 送信の成否は実際の出力ではなく、送信レジスタへ書き込めたかどうかで判断してよい。この場合、送信レジスタが
 * いっぱいであれば、送信できなかったことになる
 */
bool_t sio_snd_chr(SIOPCB *siopcb, char_t c)
{
	uint32_t reg = siopcb->reg_base;	/* UARTのベースアドレスを取得 */

	if (sio_putready(siopcb)) {
		uart_write( reg, UART_THR, c);

		return true;
	}
	else
		return false;
}

/**
 * \ingroup SIOAPI
 * \brief 1文字受信
 * \param siopcb 受信するSIOポート
 * \return 受信した文字 受信した文字を+0から+255までの正の値として返す。何も受信していないときには-1を返す。
 * \details
 * シリアル制御タスクから呼び出されて１文字受信する関数。何も受信していない時には、-1を返す。受信した文字は0から255までの正の数として返す。
 */
int_t sio_rcv_chr(SIOPCB *siopcb)
{

	int_t c = -1;
	uint32_t reg = siopcb->reg_base;	/* UARTのベースアドレスを取得 */

	if (sio_getready(siopcb)) {
		c = uart_read( reg, UART_RBR ) & 0xFF;
	}

	return c;
}

/**
 *  \ingroup SIOAPI
 *  \brief コールバックの許可
 *  \param siopcb コールバックを許可するSIOポート
 *  \param cbrtn SIO_RDY_SND あるいは SIO_RDY_RCV
 *  \details
 *  シリアル制御タスクのコールバック関数の呼び出しを許可する関数。 cbrtn 引数を使って送信と受信のいずれの側で許可するかを
 *  指定する。
 *
 *  この関数はTOPPERS/ASPのシリアル制御タスクの中から呼び出される。コールバックの許可にはペリフェラルの割り込み許可機能を使う。
 */
void sio_ena_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
	uint32_t reg = siopcb->reg_base;	/* UARTのベースアドレスを取得 */

	switch (cbrtn) {
	case SIO_RDY_SND:
		uart_write( reg, UART_IER, uart_read(reg, UART_IER) | IER_TX );	/* 送信割り込みを許可 */
		break;
	case SIO_RDY_RCV:
		uart_write( reg, UART_IER, uart_read(reg, UART_IER) | IER_RX );	/* 受信割り込みを許可 */
		break;
	default:
		break;
	}
}

/**
 *  \ingroup SIOAPI
 *  \brief コールバックの禁止
 *  \param siopcb コールバックを禁止するSIOポート
 *  \param cbrtn SIO_RDY_SND あるいは SIO_RDY_RCV
 *  \details
 *  シリアル制御タスクのコールバック関数の呼び出しを禁止する関数。 cbrtn 引数を使って送信と受信のいずれの側で禁止するかを
 *  指定する。
 *
 *  この関数はTOPPERS/ASPのシリアル制御タスクの中から呼び出される。コールバックの禁止にはペリフェラルの割り込み禁止機能を使う。
 */
void sio_dis_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
	uint32_t reg = siopcb->reg_base;	/* UARTのベースアドレスを取得 */

	switch (cbrtn) {
	case SIO_RDY_SND:
		uart_write( reg, UART_IER, uart_read(reg, UART_IER) & ~IER_TX );	/* 送信割り込みを禁止 */
		break;
	case SIO_RDY_RCV:
		uart_write( reg, UART_IER, uart_read(reg, UART_IER) & ~IER_RX );	/* 受信割り込みを禁止 */
		break;
	default:
		break;
	}
}

/**
 *  \brief 1文字出力（ポーリングでの出力）
 *  \ingroup SIOSUPPORT
 *  \param c 出力する文字
 *  \param siopid 出力するポートのID
 *  \details
 *  出力ポートにポーリングで一文字出力する。これはTOPPERS/ASPで割り込みが有効になる前に使用する関数である。
 *  ポーリングを使うので通常タスクが使ってはいけない。
 */
void sio_pol_snd_chr(char_t c, ID siopid)
{
	uint32_t reg = sioreg_table[SIOPID2INDEX(siopid)];		/* 使用するUARTのベースアドレスを取得 */

	uart_write( reg, UART_THR, c);

	while ((uart_read( reg, UART_LSR ) & LSR_TX_EMPTY) == 0)
		;
}

