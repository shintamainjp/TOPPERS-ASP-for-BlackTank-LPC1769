#ifndef PLATFORM_H
#define PLATFORM_H

void spi_init(void);
void spi_tx(unsigned char c);
unsigned char spi_rx(void);
void spi_cs_init(void);
void spi_cs_ena(void);
void spi_cs_dis(void);
unsigned char spi_cs_stat(void);
void dly_1ms(void);

#define SPI_INIT() spi_init()
#define SPI_TX(c) spi_tx(c)
#define SPI_RX() spi_rx()
#define SPI_CS_ENA() spi_cs_ena()
#define SPI_CS_DIS() spi_cs_dis()
#define SPI_CS_STAT() spi_cs_stat()

#define FORWARD(d)
#define DLY_1MS() dly_1ms()

#endif

