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
	/**
	 * \brief DMA用のリンクリストの構成要素型
	 * \details
	 * LP17xxの GPDMAはスキャッタ、ギャザーにも使用できるリンクリストを
	 * 構成できる。LLI型はそのリストの構成単位となるデータ型である。基本的にLLIは
	 * LPC1768のユーザーズマニュアル、UM10360で記述されているDMA LLIそのままである。
	 * 詳しくはユーザーズマニュアル参照の事。
	 */
struct LLI {
	int * SrcAddr;			/**< DMAのソース・バッファ・アドレス */
	int * DstAddr;			/**< DMAのデスティネーション・バッファ・アドレス・ */
	struct LLI * nextLLI;	/**< 次のLLI要素へのポインタ */
	unsigned int Control;	/**< DMAコントロール・レジスタへのロード値 */
};

/**
 * \brief I2S用データ
 * \details
 * DMA転送などに使うデータを一ヶ所にあつめたもの。
 */
struct I2S_AUDIO_DATA {
	/**
	 * \brief 送信DMA用のLLIリスト
	 * \details
	 * LLIチェーンは2要素をもち、全体でサーキュラーバッファを構成する。初期化は \ref i2s_dma_init で行う。
	 */
	struct LLI txI2SLLI[2];

	/**
	 * \brief 受信DMA用のLLIリスト
	 * \details
	 * LLIチェーンは2要素をもち、全体でサーキュラーバッファを構成する。初期化は \ref i2s_dma_init で行う。
	 */
	struct LLI rxI2SLLI[2];

	/**
	 * \brief I2S送信DMAのバッファ
	 * \details
	 * DMAを使ってI2Sポートから送信するためのデータバッファ。二つのバッファを持つのは、ピンポン（ダブルバッファ）
	 * 制御を行うためである。DMA転送中にプログラムが次の送信データを書き込むためのバッファを取得するには、
	 * \ref i2s_getTxBuf 関数を使う。
	 */
	AUDIOSAMPLE txBuffer[2][AUDIOBUFSIZE];

	/**
	 * \brief I2S受信DMAのバッファ
	 * \details
	 * DMAを使ってI2Sポートから受信するためのデータバッファ。二つのバッファを持つのは、ピンポン（ダブルバッファ）
	 * 制御を行うためである。DMA転送中にプログラムが直前の受信データを読み込むためのバッファを取得するには、
	 * \ref i2s_getRxBuf 関数を使う。
	 */
	AUDIOSAMPLE rxBuffer[2][AUDIOBUFSIZE];

	/**
	 * \brief \ref process_audio へ渡すためのデータバッファ
	 * \details
	 * audio_processing関数は右と左のチャンネルを分離して受けとるが、TLV320AIC23BはLRLRLR...形式で
	 * データを送ってくる。そのため、あらかじめ並び替えて渡すためのバッファである。
	 */
	AUDIOSAMPLE inputBuffer[2][AUDIOBUFSIZE/2];
	/**
	 * \brief \ref process_audio からデータを受け取るためのデータバッファ
	 * \details
	 * audio_processing関数は右と左のチャンネルを分離して生成するが、TLV320AIC23BはLRLRLR...形式で
	 * データを受信する。そのため、一旦受け取って並べ替えるためのバッファである。
	 */
	AUDIOSAMPLE outputBuffer[2][AUDIOBUFSIZE/2];
};

/**
 * \brief I2S用データ
 * \details
 * DMAのLLIやバッファなどはすべてこの変数にパッケージしている。こうすることで、グローバル空間に名前が散らかることを阻止できる。
 */
static struct I2S_AUDIO_DATA audio_data;

void i2s_init()
{

		/* I2Sの電源をオン */
	LPC_SC->PCONP |= 1<< 27;
		/*  I2SのPCLKを 1/4 にセット (25MHz) */
	LPC_SC->PCLKSEL0 &= ~( 0x3 << 22);

		/** P0.4 - P0.9を I2Sに */
	LPC_PINCON->PINSEL0 &= ~(0xFFF << 8);
//	LPC_PINCON->PINSEL0 |= 0x555 << 8;
	LPC_PINCON->PINSEL0 |= 0x550 << 8;	// RX ws/clk disconnect

		/*
		 * I2Sのモード設定。
		 * I2S, 32bit, 4-wire transmitter slave mode sharing the receiver bit clock and WS
		 * Typical receiver slave mode
		 * stereo, stop & reset.
		 *
		 * TXのビット/ワードクロックはRXから供給を受け、RXのビット/ワードクロックは外部から供給を受ける。
		 * この場合、TXはデバイダ関係の設定が不要のはずである。また、UM10360 rev 2 Fig111によると
		 * I2SRXBITRATEはdiv by 1になるよう設定しなければならない。
		 *
		 */

		/* Digital Audio Output & Input*/
		/*
		 * UM10360には詳細がしるされていないが、NXPによるCMSISの例題を読む限り、
		 * 設定前にDAO/DAIをreset, stop状態に置き、処理開始時にresetから解除し、
		 * stopから解除する動作になっている。reset状態でレジスタを操作する事には
		 * 不安を感じるが、例題しか情報源がないのでそれにしたがう。
		 */
	LPC_I2S->I2SDAO = 0x0; /* Disable */
	LPC_I2S->I2SDAI = 0x0; /* Disable */


	LPC_I2S->I2SDAO =  3 << 0	// 32bit data
						| 0 << 2	// mono : 1 is mono, 0 is stereo
						| 1 << 3	// stop : 1 is stop
						| 1 << 4	// reset : 1 is reset;
						| 1 << 5	// ws_sel : 1 is slave, 0 is master
						| 31 << 6	// ws_halfpeiod : half period value -1;
						| 0 << 15;	// mute : 1 is mute, 0 is unmute

	LPC_I2S->I2SDAI =  3 << 0	// 32bit data
						| 0 << 2	// mono : 1 is mono, 0 is stereo
						| 1 << 3	// stop : 1 is stop
						| 1 << 4	// reset : 1 is reset;
						| 1 << 5	// ws_sel : 1 is slave, 0 is master
						| 31 << 6;	// ws_halfpeiod : half period value -1;

		/* DMA Configuration. RX DMA is Enabled */
	LPC_I2S->I2SDMA1 = 1 << 0	// RX enable
						| 2 << 8;	// FIFO depth = 2
		/* DMA Configuration. TX DMA is enabled */
	LPC_I2S->I2SDMA2 = 1 << 1	// TX DMA Enable
						| 2 << 16;	// FIFO depth = 2;



		/* I2SRXBITRATE */
		/*
		 * 外部ビットクロックから内部ビットクロックを生成する分周器。1で割る。
		 */
	LPC_I2S->I2SRXBITRATE = 0;				/* RX bit clock div by 1 */
	LPC_I2S->I2STXBITRATE = 0;				/* RX bit clock div by 1 */

		/* 送信モード*/
	LPC_I2S->I2STXMODE = 0;					/* Transmit slave typical mode  */
		/* 受信モード*/
	LPC_I2S->I2SRXMODE = 4;					/* Receive slave typical mode */
//	LPC_I2S->I2SRXMODE = 0;					/* Receive slave 4-wire mode */



}



void i2s_start()
{
		/* TX I2S をリセットから開放 */
	LPC_I2S->I2SDAO &= ~(1<<4 );
		/* RX I2S をリセットから開放 */
	LPC_I2S->I2SDAI &= ~(1<<4 );
		/* TX I2S をストップから開放 */
	LPC_I2S->I2SDAO &= ~(1<<3);
		/* RX I2S をストップから開放 */
	LPC_I2S->I2SDAI &= ~(1<<3 );
}



void i2s_dma_init()
{
	int i;

		/* DMA の電源をオン */
	LPC_SC->PCONP |= 1<< 29;
		/* DMA Controler Enable */
	LPC_GPDMA->DMACConfig = 0x01;			/* Little Endian, Enable*/


		/* 送信LLIの初期化*/
	audio_data.txI2SLLI[0].SrcAddr = audio_data.txBuffer[0];				/* 送信データのバッファ */
	audio_data.txI2SLLI[0].DstAddr = (int *)&LPC_I2S->I2STXFIFO;		/* I2SのFIFOに送信する */
	audio_data.txI2SLLI[0].nextLLI = &audio_data.txI2SLLI[1];			/* 次のLLIへのリンク */
		/* No interrupt, Source Incr, Src/Dest 32bit, Src/Dest Bust=1, Bufsize = AUDIOBUFSIZE */
	audio_data.txI2SLLI[0].Control = 0 << 31			// I : Interrupt : Do not trigger interrupt
							| 0 <<27 			// DI : Destination Increment : Do not increment after transfer
							| 1 <<26 			// SI : Source increment : Increment after transfer
							| 2 <<21 			// Dwidth : Destination width : 32bit
							| 2 <<18 			// SWidth : Source width : 32bit
							| 0 <<15 			// DBSize : Destination Burst
							| 0 <<12 			// SBSize : Source burst
							| AUDIOBUFSIZE;	// Transfer size [count]

		/* 送信LLIの初期化*/
	audio_data.txI2SLLI[1].SrcAddr = audio_data.txBuffer[1];				/* 送信データのバッファ */
	audio_data.txI2SLLI[1].DstAddr = (int *) &LPC_I2S->I2STXFIFO;	/* I2SのFIFOに送信する */
	audio_data.txI2SLLI[1].nextLLI = &audio_data.txI2SLLI[0];			/* 次のLLIへのリンク */
		/* No interrupt, Source Incr, Src/Dest 32bit, Src/Dest Bust=1, Bufsize = AUDIOBUFSIZE */
	audio_data.txI2SLLI[1].Control = 0 << 31			// I : Interrupt : Do not trigger interrupt
							| 0 <<27 			// DI : Destination Increment : Do not increment after transfer
							| 1 <<26 			// SI : Source increment : Increment after transfer
							| 2 <<21 			// Dwidth : Destination width : 32bit
							| 2 <<18 			// SWidth : Source width : 32bit
							| 0 <<15 			// DBSize : Destination Burst
							| 0 <<12 			// SBSize : Source burst
							| AUDIOBUFSIZE;	// Transfer size [count]


		/* 受信LLIの初期化*/
	audio_data.rxI2SLLI[0].DstAddr = audio_data.rxBuffer[0];				/* 受信データのバッファ */
	audio_data.rxI2SLLI[0].SrcAddr = (int *)&LPC_I2S->I2SRXFIFO;		/* I2SのFIFOから受信する */
	audio_data.rxI2SLLI[0].nextLLI = &audio_data.rxI2SLLI[1];			/* 次のLLIへのリンク */
		/* No interrupt, Source Incr, Src/Dest 32bit, Src/Dest Bust=1, Bufsize = AUDIOBUFSIZE */
	audio_data.rxI2SLLI[0].Control = 1 << 31			// I : Interrupt : trigger interrupt
							| 1 <<27 			// DI : Destination Increment : Increment after transfer
							| 0 <<26 			// SI : Source increment : Do not increment after transfer
							| 2 <<21 			// Dwidth : Destination width : 32bit
							| 2 <<18 			// SWidth : Source width : 32bit
							| 0 <<15 			// DBSize : Destination Burst
							| 0 <<12 			// SBSize : Source burst
							| AUDIOBUFSIZE;	// Transfer size [count]

		/* 受信LLIの初期化*/
	audio_data.rxI2SLLI[1].DstAddr = audio_data.rxBuffer[1];				/* 受信データのバッファ */
	audio_data.rxI2SLLI[1].SrcAddr = (int *) &LPC_I2S->I2SRXFIFO;	/* I2SのFIFOから受信する */
	audio_data.rxI2SLLI[1].nextLLI = &audio_data.rxI2SLLI[0];			/* 次のLLIへのリンク */
		/* No interrupt, Source Incr, Src/Dest 32bit, Src/Dest Bust=1, Bufsize = AUDIOBUFSIZE */
	audio_data.rxI2SLLI[1].Control = 1 << 31			// I : Interrupt : trigger interrupt
							| 1 <<27 			// DI : Destination Increment : Increment after transfer
							| 0 <<26 			// SI : Source increment : Do not increment after transfer
							| 2 <<21 			// Dwidth : Destination width : 32bit
							| 2 <<18 			// SWidth : Source width : 32bit
							| 0 <<15 			// DBSize : Destination Burst
							| 0 <<12 			// SBSize : Source burst
							| AUDIOBUFSIZE;	// Transfer size [count]





		/* 送信バッファのクリア */
	for( i=0; i<AUDIOBUFSIZE; i++){
		audio_data.txBuffer[0][i] =  0x00;	/* ゼロフィル */
		audio_data.txBuffer[1][i] =  0x00;
	}


		/* 受信バッファのクリア */
	for( i=0; i<AUDIOBUFSIZE; i++){
		audio_data.rxBuffer[0][i] =  0x00;	/* ゼロフィル */
		audio_data.rxBuffer[1][i] =  0x00;
	}

		/* DMAの最初の1回の動作はレジスタを直接初期化して起動する。LPC1768のDMA起動方法は
		 * 洗練されているとは言い難い */
		/* 送信DMAの起動準備 */
	LPC_GPDMACH0->DMACCSrcAddr = (int)audio_data.txI2SLLI[0].SrcAddr;
	LPC_GPDMACH0->DMACCDestAddr = (int)audio_data.txI2SLLI[0].DstAddr;
	LPC_GPDMACH0->DMACCLLI = (int)&audio_data.txI2SLLI[1];
	LPC_GPDMACH0->DMACCControl = audio_data.txI2SLLI[0].Control;

		/* 受信DMAの起動準備 */
	LPC_GPDMACH1->DMACCSrcAddr = (int)audio_data.rxI2SLLI[0].SrcAddr;
	LPC_GPDMACH1->DMACCDestAddr = (int)audio_data.rxI2SLLI[0].DstAddr;
	LPC_GPDMACH1->DMACCLLI = (int)&audio_data.rxI2SLLI[1];
	LPC_GPDMACH1->DMACCControl = audio_data.rxI2SLLI[0].Control;




	/* DMAチャンネルの設定を行う。この時点ではDMAはディセーブルである */
		/* TX */
	LPC_GPDMACH0->DMACCConfig =
								  0 		// Enable : 0 is Disable
								| 0 << 1	// Src Peripheral ( Ignored when M->P )
								| 6 << 6	// Dst Peripheral, Dest is I2Sch1
								| 1 << 11	// Transfer type : 1 is M->P
								| 0 << 14	// IE : Interrupt Enable : 0 is Disable
								| 0 << 15 	// ITC : Terminal Count Intr Mask : 0 is masking out
								| 0 << 18; 	// H : Halt : 0 is active, 1 is Halt
		/* RX */
	LPC_GPDMACH1->DMACCConfig =
								  0 		// Enable : 0 is Disable
								| 5 << 1	// Src Peripheral, Src is I2Sch0
								| 0 << 6	// Dst Peripheral, ( Ignored when P->M )
								| 2 << 11	// Transfer type : 2 is P->M
								| 1 << 14	// IE : Interrupt Enable : 0 is Disable
								| 1 << 15 	// ITC : Terminal Count Intr Mask : 0 is masking out
								| 0 << 18; 	// H : Halt : 0 is active, 1 is Halt

		/* 最後の処理としてDMAチャンネルを有功にする */
	LPC_GPDMACH0->DMACCConfig |=  1	;
	LPC_GPDMACH1->DMACCConfig |=  1	;

}


/**
 * \brief 現在ソフトウェアが使えるI2S TXバッファをかえす。
 * \return プログラムが書き込んでいい送信バッファの先頭アドレス
 * \details
 * DMAが使用していないバッファをかえす。DMAと同期したプログラムを書く場合には必ず
 * このルーチンを使用してプログラムが使ってもよいバッファを決定する。
 * \todo
 * DMAチャンネルの番号は決め打ちである。適切なDMA管理機構を使った方式への変更が必要である。
 */
AUDIOSAMPLE * i2s_getTxBuf()
{
	struct LLI * nextLLI;

		// このプログラムではDMACH0をI2S TXに使っている。
		// 使用していないバッファとは、現在使用中のLLIの、次のLLIが指し示すバッファである。
	nextLLI = (struct LLI *)LPC_GPDMACH0->DMACCLLI;
	return nextLLI->SrcAddr;
}


/**
 * \brief 現在ソフトウェアが使えるI2S RXバッファをかえす。
 * \return プログラムが読み込んでいい受信バッファの先頭アドレス
 * \details
 * DMAが使用していないバッファをかえす。DMAと同期したプログラムを書く場合には必ず
 * このルーチンを使用してプログラムが使ってもよいバッファを決定する。
 * \todo
 * DMAチャンネルの番号は決め打ちである。適切なDMA管理機構を使った方式への変更が必要である。
 */
AUDIOSAMPLE * i2s_getRxBuf()
{
	struct LLI * nextLLI;

		// このプログラムではDMACH1をI2S RXに使っている。
		// 使用していないバッファとは、現在使用中のLLIの、次のLLIが指し示すバッファである。
	nextLLI = (struct LLI *)LPC_GPDMACH1->DMACCLLI;
	return nextLLI->DstAddr;
}


void i2s_dma_intr_handler()
{
	if ( LPC_GPDMA->DMACIntTCStat & ( 1<<1 ))	// I2S DMAバッファ終了割り込みか？
	{
			// CH1 (I2S DMA)のTC割り込みをクリア
		LPC_GPDMA->DMACIntTCClear = 1<<1;
			// IS2 バッファ転送割り込みが起きたことをタスクに通知する。
		isig_sem(SEM_I2SDMA);
	}
}



void audio_task(intptr_t exinf)
{
	AUDIOSAMPLE * txbuf, *rxbuf;
	int index, ch, sample;

		/* リアルタイム・ステータス用のテストピンを出力にする */
	LPC_GPIO2->FIODIR |= 1<<8;	// set test pin output

	while(1)
	{
			// DMAバッファ転送の終了を待つ
		wai_sem(SEM_I2SDMA);

			// 同期状態を示すためのテストピン信号を作成する
		LPC_GPIO2->FIOPIN ^= 1<<8;			// test pinのトグル

			// プログラムが使用してもよいバッファのアドレスを取得する。
		txbuf = i2s_getTxBuf();
		rxbuf = i2s_getRxBuf();


		index = 0;
		for ( sample=0; sample<AUDIOBUFSIZE/2; sample++)
			for ( ch=0; ch<2; ch++)
				audio_data.inputBuffer[ch][sample] = rxbuf[index++];

		process_audio( audio_data.inputBuffer, audio_data.outputBuffer, AUDIOBUFSIZE/2);

		index = 0;
		for ( sample=0; sample<AUDIOBUFSIZE/2; sample++)
			for ( ch=0; ch<2; ch++)
				txbuf[index++] = audio_data.outputBuffer[ch][sample];


	}
}
