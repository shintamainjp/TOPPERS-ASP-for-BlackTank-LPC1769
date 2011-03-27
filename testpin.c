/**
 * @file testpin.c
 * @author Shinichiro Nakamura
 * @brief テストピン制御ライブラリの実装。
 */

#include <LPC17xx.h>
#include "testpin.h"

#define TP1_PIN_NUM 25
#define TP2_PIN_NUM 26

void testpin_init(void)
{
    LPC_GPIO1->FIODIR |= (1 << TP1_PIN_NUM);
    LPC_GPIO1->FIODIR |= (1 << TP2_PIN_NUM);
}

void testpin_tp1(uint8_t on)
{
    if (on) {
        LPC_GPIO1->FIOPIN |= (1 << TP1_PIN_NUM);
    } else {
        LPC_GPIO1->FIOPIN &= ~(1 << TP1_PIN_NUM);
    }
}

void testpin_tp2(uint8_t on)
{
    if (on) {
        LPC_GPIO1->FIOPIN |= (1 << TP2_PIN_NUM);
    } else {
        LPC_GPIO1->FIOPIN &= ~(1 << TP2_PIN_NUM);
    }
}

void testpin_tp1_toggle(void)
{
    LPC_GPIO1->FIOPIN ^= (1 << TP1_PIN_NUM);
}

void testpin_tp2_toggle(void)
{
    LPC_GPIO1->FIOPIN ^= (1 << TP2_PIN_NUM);
}

