#include "platform.h"

#include <lpc17xx_spi.h>
#include <lpc17xx_gpio.h>
#include <lpc17xx_pinsel.h>
#include <lpc17xx_libcfg.h>
#include <kernel.h>

SPI_CFG_Type SPI_ConfigStruct;
PINSEL_CFG_Type PinCfg;
SPI_DATA_SETUP_Type xferConfig;

// PORT number that /CS pin assigned on
#define CS_PORT_NUM 1
// PIN number that  /CS pin assigned on
#define CS_PIN_NUM 29

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
     * P0.17 - MISO
     * P0.18 - MOSI
     */
    PinCfg.Funcnum = 3;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 15;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 17;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 18;
    PINSEL_ConfigPin(&PinCfg);

    // initialize SPI configuration structure to default
    SPI_ConfigStructInit(&SPI_ConfigStruct);
#if 0
    SPI_ConfigStruct.CPHA = SPI_CPHA_SECOND;
    SPI_ConfigStruct.CPOL = SPI_CPOL_LO;
    SPI_ConfigStruct.ClockRate = 2000;
    SPI_ConfigStruct.DataOrder = SPI_DATA_MSB_FIRST;
    SPI_ConfigStruct.Databit = SPI_DATABIT_SIZE;
#endif
    SPI_ConfigStruct.Mode = SPI_MASTER_MODE;
    // Initialize SPI peripheral with parameter given in structure above
    SPI_Init(LPC_SPI, &SPI_ConfigStruct);

    spi_cs_init();
}

void spi_tx(unsigned char c)
{
    SPI_SendData(LPC_SPI, c);
    while (!(SPI_GetStatus(LPC_SPI) & SPI_SPSR_SPIF))
    {
    }
    SPI_ReceiveData(LPC_SPI);
}

unsigned char spi_rx(void)
{
    SPI_SendData(LPC_SPI, 0xff);
    while (!(SPI_GetStatus(LPC_SPI) & SPI_SPSR_SPIF))
    {
    }
    return SPI_ReceiveData(LPC_SPI);
}

void dly_1ms(void)
{
    tslp_tsk(1);
}

