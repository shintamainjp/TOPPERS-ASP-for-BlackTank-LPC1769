/**
 * @file ffspi.h
 * @brief FATファイルシステム用下層SPIドライバの定義。
 */

#ifndef FFSPI_H
#define FFSPI_H

void ffspi_init(void);
void ffspi_tx(unsigned char c);
unsigned char ffspi_rx(void);
void ffspi_cs_init(void);
void ffspi_cs_ena(void);
void ffspi_cs_dis(void);
unsigned char ffspi_cs_stat(void);
void ffspi_dly_ms(int ms);

#endif

