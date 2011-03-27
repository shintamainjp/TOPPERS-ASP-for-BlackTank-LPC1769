/**
 * \file i2c_subsystem.c
 * \brief I2C制御関数群
 * \date 2011/02/19
 * \author Takemasa Nakamura
 */
#include "i2c_subsystem.h"

#include <LPC17xx.h>
#include <kernel.h>
#include <t_syslog.h>

void i2c_init()
{
    /* I2C0の電源をオン */
    LPC_SC->PCONP |= 1 << 7;
    /*  I2C0のPCLKを 1/4 にセット (25MHz) */
    LPC_SC->PCLKSEL0 &= ~(0x3 << 14);

    /* P0.27, P0.28を SCL0, SDA0に */
    LPC_PINCON->PINSEL1 &= ~(0xF << 22);
    LPC_PINCON->PINSEL1 |= 0x5 << 22;

    /* I2C動作クロック　100KHz (コアクロック100MHzのとき) */
    LPC_I2C0->I2SCLH = 1250;
    LPC_I2C0->I2SCLL = 1250;

    /* I2C ディセーブル */
    LPC_I2C0->I2CONCLR = 0xFF;
}

void i2c_send_2bytes(int addr, int data0, int data1)
{
    /* I2C マスターモード */
    LPC_I2C0->I2CONSET = 0x40;
    /* I2C START */
    LPC_I2C0->I2CONSET = 0x20;
    tslp_tsk(10);
    if (LPC_I2C0->I2STAT != 0x08) {
        syslog(LOG_NOTICE, "I2C0 Start. Expects 0x08 ");
        syslog(LOG_NOTICE, "I2CONSET0 %02x", LPC_I2C0->I2CONSET);
        syslog(LOG_NOTICE, "I2STAT0 %02x", LPC_I2C0->I2STAT);
    }

    /* I2C アドレス TLV320AIC23B CS=0, Write */
    LPC_I2C0->I2DAT = addr << 1;

    /* reset SIC & STA */
    LPC_I2C0->I2CONCLR = 0x28;
    tslp_tsk(10);
    if (LPC_I2C0->I2STAT != 0x18) {
        syslog(LOG_NOTICE, "SI&STA Clear, SLA + W sent. Expects 0x18 ");
        syslog(LOG_NOTICE, "I2CONSET0 %02x", LPC_I2C0->I2CONSET);
        syslog(LOG_NOTICE, "I2STAT0 %02x", LPC_I2C0->I2STAT);
    }

    /* 最初のデータの転送 */
    LPC_I2C0->I2DAT = data0;
    /* reset SIC */
    LPC_I2C0->I2CONCLR = 0x08;
    tslp_tsk(10);
    if (LPC_I2C0->I2STAT != 0x28) {
        syslog(LOG_NOTICE, "SI Clear, dataH sent. Expects 0x28 ");
        syslog(LOG_NOTICE, "I2CONSET0 %02x", LPC_I2C0->I2CONSET);
        syslog(LOG_NOTICE, "I2STAT0 %02x", LPC_I2C0->I2STAT);
    }
    /* 次のデータの転送 */
    LPC_I2C0->I2DAT = data1;
    /* reset SIC */
    LPC_I2C0->I2CONCLR = 0x08;
    tslp_tsk(10);
    if (LPC_I2C0->I2STAT != 0x28) {
        syslog(LOG_NOTICE, "SI Clear, dataL sent. Expects 0x28 ");
        syslog(LOG_NOTICE, "I2CONSET0 %02x", LPC_I2C0->I2CONSET);
        syslog(LOG_NOTICE, "I2STAT0 %02x", LPC_I2C0->I2STAT);
    }
    /* set STOP */
    LPC_I2C0->I2CONSET = 0x10;

    /* reset SIC */
    LPC_I2C0->I2CONCLR = 0x08;
    tslp_tsk(10);
    if (LPC_I2C0->I2STAT != 0xF8) {
        syslog(LOG_NOTICE, "SI Clear. Expects 0xF8, NO SI ");
        syslog(LOG_NOTICE, "I2CONSET0 %02x", LPC_I2C0->I2CONSET);
        syslog(LOG_NOTICE, "I2STAT0 %02x", LPC_I2C0->I2STAT);
    }
}
