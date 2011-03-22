#ifndef PFFSPI_H
#define PFFSPI_H

void pffspi_init(void);
void pffspi_tx(unsigned char c);
unsigned char pffspi_rx(void);
void pffspi_cs_init(void);
void pffspi_cs_ena(void);
void pffspi_cs_dis(void);
unsigned char pffspi_cs_stat(void);
void pffspi_dly_1ms(void);

#endif

