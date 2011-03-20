/**
 * \file adc_subsystem.c
 * \brief 内蔵ADC制御関数
 * \date 2011/02/19
 * \author Takemasa Nakamura
 */

#include "adc_subsystem.h"
#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

void adc_init()
{
		/* ADCの電源をオン */
	LPC_SC->PCONP |= 1<<12;
		/* ADC0-3にピンを割り付ける */
	LPC_PINCON->PINSEL1 &= ~( 0xFF << 14);
	LPC_PINCON->PINSEL1 |= 0x55 << 14;
	/* ADC0-3のプルアップ、プルダウンを外す */
	LPC_PINCON->PINMODE1 &= ~( 0xFF << 14);
	LPC_PINCON->PINMODE1 |= 0xaa << 14;

	LPC_ADC->ADCR = 	0x0F 		/* ADCの	チャンネルマスク。Use ADC0 to 3 pins */
					|	4<<8 		/* 25MHz divide by 5*/
					|	1<<16  	/* Burst mode */
					|	1<<21;  	/* PDN : 1=>Operational, 0=>PDN */
}

unsigned short adc_read( int ch)
{
	int adc_value ;

	adc_value = (&LPC_ADC->ADDR0)[ch];
	return ( adc_value & 0xFFF0 );
}
