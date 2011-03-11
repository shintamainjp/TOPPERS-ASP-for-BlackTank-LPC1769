#include "platform.h"

#include <lpc17xx_ssp.h>
#include <lpc17xx_gpio.h>
#include <lpc17xx_pinsel.h>
#include <lpc17xx_libcfg.h>
#include <kernel.h>

SSP_CFG_Type SSP_ConfigStruct;
PINSEL_CFG_Type PinCfg;
SSP_DATA_SETUP_Type xferConfig;

// PORT number that /CS pin assigned on
#define CS_PORT_NUM 0
// PIN number that  /CS pin assigned on
#define CS_PIN_NUM 16

void spi_cs_init(void)
{
    GPIO_SetDir(CS_PORT_NUM, (1 << CS_PIN_NUM), 1);
    GPIO_SetValue(CS_PORT_NUM, (1 << CS_PIN_NUM));
}

void spi_cs_ena(void)
{
    GPIO_ClearValue(CS_PORT_NUM, (1 << CS_PIN_NUM));
}

void spi_cs_dis(void)
{
    GPIO_SetValue(CS_PORT_NUM, (1 << CS_PIN_NUM));
}

/* CS status (true:CS == L) */
unsigned char spi_cs_stat(void)
{
    unsigned char val = GPIO_ReadValue(CS_PORT_NUM);
    return ((val & (1 << CS_PIN_NUM)) == 0) ? 1 : 0;
}

void spi_init(void)
{
    /*
     * Initialize SPI pin connect
     * P0.15 - SCK;
     * P0.16 - SSEL
     * P0.17 - MISO
     * P0.18 - MOSI
     */
    PinCfg.Funcnum = 2;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 15;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 17;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 18;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 16;
    PINSEL_ConfigPin(&PinCfg);

    // initialize SSP configuration structure to default
    SSP_ConfigStructInit(&SSP_ConfigStruct);
    // Initialize SSP peripheral with parameter given in structure above
    SSP_Init(LPC_SSP0, &SSP_ConfigStruct);
    // Enable SSP peripheral
    SSP_Cmd(LPC_SSP0, ENABLE);


    spi_cs_init();
}

void spi_tx(unsigned char c)
{
    SSP_SendData(LPC_SSP0, c);
    while (SSP_GetStatus(LPC_SSP0, SSP_SR_BSY) == SET)
    {
    }
    SSP_ReceiveData(LPC_SSP0);
}

unsigned char spi_rx(void)
{
    SSP_SendData(LPC_SSP0, 0xff);
    while (SSP_GetStatus(LPC_SSP0, SSP_SR_BSY) == SET)
    {
    }
    return SSP_ReceiveData(LPC_SSP0);
}

void dly_1ms(void)
{
    tslp_tsk(1);
}

