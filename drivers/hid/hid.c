
#include "hid.h"

#include <lpc17xx_gpio.h>
#include <lpc17xx_pinsel.h>
#include <lpc17xx_libcfg.h>
#include <kernel.h>

#define LED1_PORT_NUM 1
#define LED1_PIN_NUM 9

#define LED2_PORT_NUM 1
#define LED2_PIN_NUM 10

#define LED3_PORT_NUM 1
#define LED3_PIN_NUM 14

#define LED4_PORT_NUM 1
#define LED4_PIN_NUM 15

void hid_init(void)
{
    GPIO_SetDir(LED1_PORT_NUM, (1 << LED1_PIN_NUM), 1);
    GPIO_SetValue(LED1_PORT_NUM, (1 << LED1_PIN_NUM));

    GPIO_SetDir(LED2_PORT_NUM, (1 << LED2_PIN_NUM), 1);
    GPIO_SetValue(LED2_PORT_NUM, (1 << LED2_PIN_NUM));

    GPIO_SetDir(LED3_PORT_NUM, (1 << LED3_PIN_NUM), 1);
    GPIO_SetValue(LED3_PORT_NUM, (1 << LED3_PIN_NUM));

    GPIO_SetDir(LED4_PORT_NUM, (1 << LED4_PIN_NUM), 1);
    GPIO_SetValue(LED4_PORT_NUM, (1 << LED4_PIN_NUM));
}

unsigned char hid_swread(unsigned char ch)
{
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

