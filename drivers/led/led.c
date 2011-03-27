/**
 * @file led.c
 * @author Shinichiro Nakamura
 * @brief LEDドライバの実装。
 */

#include "led.h"
#include <LPC17xx.h>
#include <lpc17xx_gpio.h>
#include <lpc17xx_pinsel.h>
#include <lpc17xx_libcfg.h>

#define DEBLED1_PIN_NUM 4
#define DEBLED2_PIN_NUM 5
#define DEBLED3_PIN_NUM 6
#define DEBLED4_PIN_NUM 7

#define SWLED1_PORT_NUM 1
#define SWLED2_PORT_NUM 1
#define SWLED3_PORT_NUM 1
#define SWLED4_PORT_NUM 1

#define SWLED1_PIN_NUM 9
#define SWLED2_PIN_NUM 10
#define SWLED3_PIN_NUM 14
#define SWLED4_PIN_NUM 15

void led_init(void)
{
    LPC_GPIO2->FIOCLR = (1 << DEBLED1_PIN_NUM);
    LPC_GPIO2->FIOCLR = (1 << DEBLED2_PIN_NUM);
    LPC_GPIO2->FIOCLR = (1 << DEBLED3_PIN_NUM);
    LPC_GPIO2->FIOCLR = (1 << DEBLED4_PIN_NUM);

    LPC_GPIO2->FIODIR |= (1 << DEBLED1_PIN_NUM);
    LPC_GPIO2->FIODIR |= (1 << DEBLED2_PIN_NUM);
    LPC_GPIO2->FIODIR |= (1 << DEBLED3_PIN_NUM);
    LPC_GPIO2->FIODIR |= (1 << DEBLED4_PIN_NUM);

    GPIO_SetValue(SWLED1_PORT_NUM, (1 << SWLED1_PIN_NUM));
    GPIO_SetValue(SWLED2_PORT_NUM, (1 << SWLED2_PIN_NUM));
    GPIO_SetValue(SWLED3_PORT_NUM, (1 << SWLED3_PIN_NUM));
    GPIO_SetValue(SWLED4_PORT_NUM, (1 << SWLED4_PIN_NUM));

    GPIO_SetDir(SWLED1_PORT_NUM, (1 << SWLED1_PIN_NUM), 1);
    GPIO_SetDir(SWLED2_PORT_NUM, (1 << SWLED2_PIN_NUM), 1);
    GPIO_SetDir(SWLED3_PORT_NUM, (1 << SWLED3_PIN_NUM), 1);
    GPIO_SetDir(SWLED4_PORT_NUM, (1 << SWLED4_PIN_NUM), 1);
}

void led_debug_write(const uint8_t target, const uint8_t on)
{
    int pin;
    switch (target) {
        case 0:
            pin = (1 << DEBLED1_PIN_NUM);
            break;
        case 1:
            pin = (1 << DEBLED2_PIN_NUM);
            break;
        case 2:
            pin = (1 << DEBLED3_PIN_NUM);
            break;
        case 3:
            pin = (1 << DEBLED4_PIN_NUM);
            break;
        default:
            return;
    }
    if (on) {
        LPC_GPIO2->FIOPIN |= pin;
    } else {
        LPC_GPIO2->FIOPIN &= ~pin;
    }
}

void led_switch_write(const uint8_t target, const uint8_t on)
{
    int port;
    int pin;
    switch (target) {
        case 0:
            port = SWLED1_PORT_NUM;
            pin = SWLED1_PIN_NUM;
            break;
        case 1:
            port = SWLED2_PORT_NUM;
            pin = SWLED2_PIN_NUM;
            break;
        case 2:
            port = SWLED3_PORT_NUM;
            pin = SWLED3_PIN_NUM;
            break;
        case 3:
            port = SWLED4_PORT_NUM;
            pin = SWLED4_PIN_NUM;
            break;
        default:
            return;
    }
    if (on) {
        GPIO_ClearValue(port, (1 << pin));
    } else {
        GPIO_SetValue(port, (1 << pin));
    }
}

