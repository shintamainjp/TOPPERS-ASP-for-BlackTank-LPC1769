
#include "hid.h"

#include <lpc17xx_gpio.h>
#include <lpc17xx_pinsel.h>
#include <lpc17xx_libcfg.h>
#include <kernel.h>

#define LED1_PORT_NUM 1
#define LED2_PORT_NUM 1
#define LED3_PORT_NUM 1
#define LED4_PORT_NUM 1

#define LED1_PIN_NUM 9
#define LED2_PIN_NUM 10
#define LED3_PIN_NUM 14
#define LED4_PIN_NUM 15

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
    GPIO_SetValue(LED1_PORT_NUM, (1 << LED1_PIN_NUM));
    GPIO_SetValue(LED2_PORT_NUM, (1 << LED2_PIN_NUM));
    GPIO_SetValue(LED3_PORT_NUM, (1 << LED3_PIN_NUM));
    GPIO_SetValue(LED4_PORT_NUM, (1 << LED4_PIN_NUM));

    GPIO_SetDir(LED1_PORT_NUM, (1 << LED1_PIN_NUM), 1);
    GPIO_SetDir(LED2_PORT_NUM, (1 << LED2_PIN_NUM), 1);
    GPIO_SetDir(LED3_PORT_NUM, (1 << LED3_PIN_NUM), 1);
    GPIO_SetDir(LED4_PORT_NUM, (1 << LED4_PIN_NUM), 1);

    GPIO_SetDir(SW1_PORT_NUM, (1 << SW1_PIN_NUM), 0);
    GPIO_SetDir(SW2_PORT_NUM, (1 << SW2_PIN_NUM), 0);
    GPIO_SetDir(SW3_PORT_NUM, (1 << SW3_PIN_NUM), 0);
    GPIO_SetDir(SW4_PORT_NUM, (1 << SW4_PIN_NUM), 0);
}

unsigned char hid_swread(unsigned char ch)
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

void hid_swled(unsigned char ch, unsigned char on)
{
    int port;
    int pin;
    switch (ch) {
        case 0:
            port = LED1_PORT_NUM;
            pin = LED1_PIN_NUM;
            break;
        case 1:
            port = LED2_PORT_NUM;
            pin = LED2_PIN_NUM;
            break;
        case 2:
            port = LED3_PORT_NUM;
            pin = LED3_PIN_NUM;
            break;
        case 3:
            port = LED4_PORT_NUM;
            pin = LED4_PIN_NUM;
            break;
        default:
            return;
    }
    if (on == 1) {
        GPIO_ClearValue(port, (1 << pin));
    } else {
        GPIO_SetValue(port, (1 << pin));
    }
}

