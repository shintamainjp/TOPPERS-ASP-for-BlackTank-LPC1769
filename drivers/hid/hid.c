
#include "hid.h"

#include <lpc17xx_gpio.h>
#include <lpc17xx_adc.h>
#include <lpc17xx_pinsel.h>
#include <lpc17xx_libcfg.h>
#include <kernel.h>

#define SW1_PORT_NUM 1
#define SW2_PORT_NUM 1
#define SW3_PORT_NUM 1
#define SW4_PORT_NUM 1

#define SW1_PIN_NUM 0
#define SW2_PIN_NUM 1
#define SW3_PIN_NUM 4
#define SW4_PIN_NUM 8

void hid_init(void)
{
    PINSEL_CFG_Type PinCfg;

    GPIO_SetDir(SW1_PORT_NUM, (1 << SW1_PIN_NUM), 0);
    GPIO_SetDir(SW2_PORT_NUM, (1 << SW2_PIN_NUM), 0);
    GPIO_SetDir(SW3_PORT_NUM, (1 << SW3_PIN_NUM), 0);
    GPIO_SetDir(SW4_PORT_NUM, (1 << SW4_PIN_NUM), 0);

    /*
     * Init ADC pin connect
     * AD0.2 on P0.25
     */
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 0;

    PinCfg.Pinnum = 23;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 24;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 25;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 26;
    PINSEL_ConfigPin(&PinCfg);

    ADC_Init(LPC_ADC, 1000000);

    ADC_IntConfig(LPC_ADC, ADC_ADINTEN0, DISABLE);
    ADC_IntConfig(LPC_ADC, ADC_ADINTEN1, DISABLE);
    ADC_IntConfig(LPC_ADC, ADC_ADINTEN2, DISABLE);
    ADC_IntConfig(LPC_ADC, ADC_ADINTEN3, DISABLE);

    ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE);
    ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_1, ENABLE);
    ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_2, ENABLE);
    ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_3, ENABLE);

    ADC_BurstCmd(LPC_ADC, 1);
}

uint8_t hid_swread(unsigned char ch)
{
    int port;
    int pin;
    switch (ch) {
        case 0:
            port = SW1_PORT_NUM;
            pin = SW1_PIN_NUM;
            break;
        case 1:
            port = SW2_PORT_NUM;
            pin = SW2_PIN_NUM;
            break;
        case 2:
            port = SW3_PORT_NUM;
            pin = SW3_PIN_NUM;
            break;
        case 3:
            port = SW4_PORT_NUM;
            pin = SW4_PIN_NUM;
            break;
        default:
            return 0;
    }
    if ((GPIO_ReadValue(port) & (1 << pin)) != 0) {
        return 0;
    } else {
        return 1;
    }
}

uint32_t hid_volread(unsigned char ch)
{
    uint32_t val = 0;
    switch (ch) {
        case 0:
            val = ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_0);
            break;
        case 1:
            val = ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_1);
            break;
        case 2:
            val = ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_2);
            break;
        case 3:
            val = ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_3);
            break;
    }
    return val;
}

