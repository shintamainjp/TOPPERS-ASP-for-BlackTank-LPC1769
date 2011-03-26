/**
 * \file i2s_subsystem.c
 * \brief I2S制御関数群
 * \date 2011/02/19
 * \author takemasa
 */

#include "i2s_subsystem.h"
#include "kernel_cfg.h"

#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

void i2s_init()
{
    /* I2Sの電源をオン */
    LPC_SC->PCONP |= 1 << 27;
    /*  I2SのPCLKを 1/4 にセット (25MHz) */
    LPC_SC->PCLKSEL0 &= ~(0x3 << 22);

    /** P0.4 - P0.9を I2Sに */
    LPC_PINCON->PINSEL0 &= ~(0xFFF << 8);
//      LPC_PINCON->PINSEL0 |= 0x555 << 8;
    LPC_PINCON->PINSEL0 |= 0x550 << 8;	// RX ws/clk disconnect

    /*
     * I2Sのモード設定。
     * I2S, 32bit, 4-wire transmitter slave mode sharing the
     * receiver bit clock and WS Typical receiver slave mode
     * stereo, stop & reset.
     *
     * TXのビット/ワードクロックはRXから供給を受け、RXの
     * ビット/ワードクロックは外部から供給を受ける。
     * この場合、TXはデバイダ関係の設定が不要のはずである。
     * また、UM10360 rev 2 Fig111によるとI2SRXBITRATEは
     * div by 1になるよう設定しなければならない。
     */

    /*
     * Digital Audio Output & Input
     */

    /*
     * UM10360には詳細がしるされていないが、NXPによるCMSISの例題を
     * 読む限り、設定前にDAO/DAIをreset, stop状態に置き、
     * 処理開始時にresetから解除し、stopから解除する動作になっている。
     * reset状態でレジスタを操作する事には不安を感じるが、例題しか
     * 情報源がないのでそれにしたがう。
     */
    LPC_I2S->I2SDAO = 0x0;	/* Disable */
    LPC_I2S->I2SDAI = 0x0;	/* Disable */

    LPC_I2S->I2SDAO = 3 << 0	// 32bit data
	| 0 << 2		// mono : 1 is mono, 0 is stereo
	| 1 << 3		// stop : 1 is stop
	| 1 << 4		// reset : 1 is reset;
	| 1 << 5		// ws_sel : 1 is slave, 0 is master
	| 31 << 6		// ws_halfpeiod : half period value -1;
	| 0 << 15;		// mute : 1 is mute, 0 is unmute

    LPC_I2S->I2SDAI = 3 << 0	// 32bit data
	| 0 << 2		// mono : 1 is mono, 0 is stereo
	| 1 << 3		// stop : 1 is stop
	| 1 << 4		// reset : 1 is reset;
	| 1 << 5		// ws_sel : 1 is slave, 0 is master
	| 31 << 6;		// ws_halfpeiod : half period value -1;

    /* DMA Configuration. RX DMA is Enabled */
    LPC_I2S->I2SDMA1 = 1 << 0	// RX enable
	| 2 << 8;		// FIFO depth = 2
    /* DMA Configuration. TX DMA is enabled */
    LPC_I2S->I2SDMA2 = 1 << 1	// TX DMA Enable
	| 2 << 16;		// FIFO depth = 2;

    /*
     * I2SRXBITRATE
     */

    /*
     * 外部ビットクロックから内部ビットクロックを生成する分周器。1で割る。
     */
    LPC_I2S->I2SRXBITRATE = 0;	/* RX bit clock div by 1 */
    LPC_I2S->I2STXBITRATE = 0;	/* RX bit clock div by 1 */

    /* 送信モード */
    LPC_I2S->I2STXMODE = 0;	/* Transmit slave typical mode  */
    /* 受信モード */
    LPC_I2S->I2SRXMODE = 4;	/* Receive slave typical mode */
//      LPC_I2S->I2SRXMODE = 0;                                 /* Receive slave 4-wire mode */

}

void i2s_start()
{
    /* TX I2S をストップから開放 */
    LPC_I2S->I2SDAO &= ~(1 << 3);
    /* RX I2S をストップから開放 */
    LPC_I2S->I2SDAI &= ~(1 << 3);
    /* TX I2S をリセットから開放 */
    LPC_I2S->I2SDAO &= ~(1 << 4);
    /* RX I2S をリセットから開放 */
    LPC_I2S->I2SDAI &= ~(1 << 4);
}

void i2s_dma_init(struct I2S_AUDIO_DATA *audio_data)
{
    int i;

    /* DMA の電源をオン */
    LPC_SC->PCONP |= 1 << 29;
    /* DMA Controler Enable */
    LPC_GPDMA->DMACConfig = 0x01;	/* Little Endian, Enable */

    /* 送信LLIの初期化 */
    audio_data->txI2SLLI[0].SrcAddr = audio_data->txBuffer[0];	/* 送信データのバッファ */
    audio_data->txI2SLLI[0].DstAddr = (int *) &LPC_I2S->I2STXFIFO;	/* I2SのFIFOに送信する */
    audio_data->txI2SLLI[0].nextLLI = &(audio_data->txI2SLLI[1]);	/* 次のLLIへのリンク */
    /* No interrupt, Source Incr, Src/Dest 32bit, Src/Dest Bust=1, Bufsize = AUDIOBUFSIZE */
    audio_data->txI2SLLI[0].Control = 0 << 31	// I : Interrupt : Do not trigger interrupt
	| 0 << 27		// DI : Destination Increment : Do not increment after transfer
	| 1 << 26		// SI : Source increment : Increment after transfer
	| 2 << 21		// Dwidth : Destination width : 32bit
	| 2 << 18		// SWidth : Source width : 32bit
	| 0 << 15		// DBSize : Destination Burst
	| 0 << 12		// SBSize : Source burst
	| AUDIOBUFSIZE;		// Transfer size [count]

    /* 送信LLIの初期化 */
    audio_data->txI2SLLI[1].SrcAddr = audio_data->txBuffer[1];	/* 送信データのバッファ */
    audio_data->txI2SLLI[1].DstAddr = (int *) &LPC_I2S->I2STXFIFO;	/* I2SのFIFOに送信する */
    audio_data->txI2SLLI[1].nextLLI = &(audio_data->txI2SLLI[0]);	/* 次のLLIへのリンク */
    /* No interrupt, Source Incr, Src/Dest 32bit, Src/Dest Bust=1, Bufsize = AUDIOBUFSIZE */
    audio_data->txI2SLLI[1].Control = 0 << 31	// I : Interrupt : Do not trigger interrupt
	| 0 << 27		// DI : Destination Increment : Do not increment after transfer
	| 1 << 26		// SI : Source increment : Increment after transfer
	| 2 << 21		// Dwidth : Destination width : 32bit
	| 2 << 18		// SWidth : Source width : 32bit
	| 0 << 15		// DBSize : Destination Burst
	| 0 << 12		// SBSize : Source burst
	| AUDIOBUFSIZE;		// Transfer size [count]

    /* 受信LLIの初期化 */
    audio_data->rxI2SLLI[0].DstAddr = audio_data->rxBuffer[0];	/* 受信データのバッファ */
    audio_data->rxI2SLLI[0].SrcAddr = (int *) &LPC_I2S->I2SRXFIFO;	/* I2SのFIFOから受信する */
    audio_data->rxI2SLLI[0].nextLLI = &(audio_data->rxI2SLLI[1]);	/* 次のLLIへのリンク */
    /* No interrupt, Source Incr, Src/Dest 32bit, Src/Dest Bust=1, Bufsize = AUDIOBUFSIZE */
    audio_data->rxI2SLLI[0].Control = 1 << 31	// I : Interrupt : trigger interrupt
	| 1 << 27		// DI : Destination Increment : Increment after transfer
	| 0 << 26		// SI : Source increment : Do not increment after transfer
	| 2 << 21		// Dwidth : Destination width : 32bit
	| 2 << 18		// SWidth : Source width : 32bit
	| 0 << 15		// DBSize : Destination Burst
	| 0 << 12		// SBSize : Source burst
	| AUDIOBUFSIZE;		// Transfer size [count]

    /* 受信LLIの初期化 */
    audio_data->rxI2SLLI[1].DstAddr = audio_data->rxBuffer[1];	/* 受信データのバッファ */
    audio_data->rxI2SLLI[1].SrcAddr = (int *) &LPC_I2S->I2SRXFIFO;	/* I2SのFIFOから受信する */
    audio_data->rxI2SLLI[1].nextLLI = &(audio_data->rxI2SLLI[0]);	/* 次のLLIへのリンク */
    /* No interrupt, Source Incr, Src/Dest 32bit, Src/Dest Bust=1, Bufsize = AUDIOBUFSIZE */
    audio_data->rxI2SLLI[1].Control = 1 << 31	// I : Interrupt : trigger interrupt
	| 1 << 27		// DI : Destination Increment : Increment after transfer
	| 0 << 26		// SI : Source increment : Do not increment after transfer
	| 2 << 21		// Dwidth : Destination width : 32bit
	| 2 << 18		// SWidth : Source width : 32bit
	| 0 << 15		// DBSize : Destination Burst
	| 0 << 12		// SBSize : Source burst
	| AUDIOBUFSIZE;		// Transfer size [count]

    /* 送信バッファのクリア */
    for (i = 0; i < AUDIOBUFSIZE; i++) {
        audio_data->txBuffer[0][i] = 0x00;	/* ゼロフィル */
        audio_data->txBuffer[1][i] = 0x00;
    }

    /* 受信バッファのクリア */
    for (i = 0; i < AUDIOBUFSIZE; i++) {
        audio_data->rxBuffer[0][i] = 0x00;	/* ゼロフィル */
        audio_data->rxBuffer[1][i] = 0x00;
    }

    /*
     * DMAの最初の1回の動作はレジスタを直接初期化して起動する。
     * LPC1768のDMA起動方法は洗練されているとは言い難い。
     */

    /* 送信DMAの起動準備 */
    LPC_GPDMACH0->DMACCSrcAddr = (int) audio_data->txI2SLLI[0].SrcAddr;
    LPC_GPDMACH0->DMACCDestAddr = (int) audio_data->txI2SLLI[0].DstAddr;
    LPC_GPDMACH0->DMACCLLI = (int) &audio_data->txI2SLLI[1];
    LPC_GPDMACH0->DMACCControl = audio_data->txI2SLLI[0].Control;

    /* 受信DMAの起動準備 */
    LPC_GPDMACH1->DMACCSrcAddr = (int) audio_data->rxI2SLLI[0].SrcAddr;
    LPC_GPDMACH1->DMACCDestAddr = (int) audio_data->rxI2SLLI[0].DstAddr;
    LPC_GPDMACH1->DMACCLLI = (int) &(audio_data->rxI2SLLI[1]);
    LPC_GPDMACH1->DMACCControl = audio_data->rxI2SLLI[0].Control;

    /*
     * DMAチャンネルの設定を行う。
     * この時点ではDMAはディセーブルである。
     */

    /* TX */
    LPC_GPDMACH0->DMACCConfig = 0	// Enable : 0 is Disable
	| 0 << 1		// Src Peripheral ( Ignored when M->P )
	| 6 << 6		// Dst Peripheral, Dest is I2Sch1
	| 1 << 11		// Transfer type : 1 is M->P
	| 0 << 14		// IE : Interrupt Enable : 0 is Disable
	| 0 << 15		// ITC : Terminal Count Intr Mask : 0 is masking out
	| 0 << 18;		// H : Halt : 0 is active, 1 is Halt
    /* RX */
    LPC_GPDMACH1->DMACCConfig = 0	// Enable : 0 is Disable
	| 5 << 1		// Src Peripheral, Src is I2Sch0
	| 0 << 6		// Dst Peripheral, ( Ignored when P->M )
	| 2 << 11		// Transfer type : 2 is P->M
	| 1 << 14		// IE : Interrupt Enable : 0 is Disable
	| 1 << 15		// ITC : Terminal Count Intr Mask : 0 is masking out
	| 0 << 18;		// H : Halt : 0 is active, 1 is Halt

    /* 最後の処理としてDMAチャンネルを有功にする */
    LPC_GPDMACH0->DMACCConfig |= 1;
    LPC_GPDMACH1->DMACCConfig |= 1;
}

/**
 * \brief 現在ソフトウェアが使えるI2S TXバッファをかえす。
 * \return プログラムが書き込んでいい送信バッファの先頭アドレス
 * \details
 * DMAが使用していないバッファをかえす。
 * DMAと同期したプログラムを書く場合には必ずこのルーチンを使用して
 * プログラムが使ってもよいバッファを決定する。
 * \todo
 * DMAチャンネルの番号は決め打ちである。
 * 適切なDMA管理機構を使った方式への変更が必要である。
 */
AUDIOSAMPLE *i2s_getTxBuf()
{
    struct LLI *nextLLI;

    // このプログラムではDMACH0をI2S TXに使っている。
    // 使用していないバッファとは、現在使用中のLLIの、
    // 次のLLIが指し示すバッファである。
    nextLLI = (struct LLI *) LPC_GPDMACH0->DMACCLLI;
    return nextLLI->SrcAddr;
}

/**
 * \brief 現在ソフトウェアが使えるI2S RXバッファをかえす。
 * \return プログラムが読み込んでいい受信バッファの先頭アドレス
 * \details
 * DMAが使用していないバッファをかえす。
 * DMAと同期したプログラムを書く場合には必ずこのルーチンを使用して
 * プログラムが使ってもよいバッファを決定する。
 * \todo
 * DMAチャンネルの番号は決め打ちである。
 * 適切なDMA管理機構を使った方式への変更が必要である。
 */
AUDIOSAMPLE *i2s_getRxBuf()
{
    struct LLI *nextLLI;

    // このプログラムではDMACH1をI2S RXに使っている。
    // 使用していないバッファとは、現在使用中のLLIの、
    // 次のLLIが指し示すバッファである。
    nextLLI = (struct LLI *) LPC_GPDMACH1->DMACCLLI;
    return nextLLI->DstAddr;
}

void i2s_dma_intr_handler()
{
    if (LPC_GPDMA->DMACIntTCStat & (1 << 1))
    {
        /*
         * I2S DMAバッファ終了割り込み
         */
        // CH1 (I2S DMA)のTC割り込みをクリア
        LPC_GPDMA->DMACIntTCClear = (1 << 1);
        // IS2 バッファ転送割り込みが起きたことをタスクに通知する。
        isig_sem(SEM_I2SDMA);
    }
}

