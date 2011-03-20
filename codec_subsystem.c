/**
 * \file codec_subsystem.c
 * \brief TLV320AIC23B操作関数
 * \date 2011/02/19
 * \author takemasa
 */

#include "codec_subsystem.h"
#include "i2c_subsystem.h"

#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

/**
 * \brief コーデック初期化データ列の長さ
 * \details
 * TLV320AIC23に与える初期化データの長さである。長さはレジスタアドレスとデータの組が
 * 何組であるかを示す。
 */
#define 	CODECINITDATALEN	10	/* TLV320AIC23B初期化データ長 */


/**
 * \brief CODEC初期化データ列
 * \details
 * TLV320AIC23Bを初期化するためのデータ列である。
 */
static unsigned char tlv320aic23init[CODECINITDATALEN][2] =
{
		{ 0 | 0x01, 0x17 },	/* L_in vol : LR simul-update, unmute, 0dB */
		{ 2 | 0x01, 0x17 },	/* R_in vol : LR simul-update, unmute, 0dB */
		{ 4 | 0x01, 0xF9 },	/* L_HP vol : LR simul-update, zero-cross, 0dB */
		{ 6 | 0x01, 0xF9 },	/* R_HP vol : LR simul-update, zero-cross, 0dB */
		{ 8 | 0x00, 0x12 },	/* Analog Audio Path : No Sidetone, No bypass, DAC for Out, Line out for ADC, Mic Mute */
		{ 10 | 0x00, 0x06 },	/* Digital Paht: DAC unmute, De-emphasis 48k, ADC LPF enable */
		{ 12 | 0x00, 0x02 }, 	/* Power Down : Only Mic is down*/
		{ 14 | 0x00, 0x4E },	/* Digital Audio Format : Master, 24bit, I2S */
//		{ 16 | 0x00, 0x01 }, 	/* Sanmpling Rate, 48kHz, USB mode*/
		{ 16 | 0x00, 0x00 }, 	/* Sanmpling Rate, 48kHz, NORMAL mode*/
		{ 18 | 0x00, 0x01 }	/* Activateion : Active. */

};

void codec_init()
{
	int i;

		/* TLV320AIC23Bに初期化データを送信する */
	for ( i=0; i< CODECINITDATALEN; i++){
		i2c_send_2bytes( 0x1A, tlv320aic23init[i][0], tlv320aic23init[i][1]);		/* Activate all functionality of TLV320AIC23B */
                tslp_tsk(10);
	}
}
