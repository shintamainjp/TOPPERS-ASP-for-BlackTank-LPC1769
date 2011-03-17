#include "debled.h"
#include <LPC17xx.h>

#define LED1_PIN_NUM 4
#define LED2_PIN_NUM 5
#define LED3_PIN_NUM 6
#define LED4_PIN_NUM 7

void debled_init(void)
{
    LPC_GPIO2->FIOCLR = (1 << LED1_PIN_NUM);
    LPC_GPIO2->FIOCLR = (1 << LED2_PIN_NUM);
    LPC_GPIO2->FIOCLR = (1 << LED3_PIN_NUM);
    LPC_GPIO2->FIOCLR = (1 << LED4_PIN_NUM);

    LPC_GPIO2->FIODIR |= (1 << LED1_PIN_NUM);
    LPC_GPIO2->FIODIR |= (1 << LED2_PIN_NUM);
    LPC_GPIO2->FIODIR |= (1 << LED3_PIN_NUM);
    LPC_GPIO2->FIODIR |= (1 << LED4_PIN_NUM);
}

void debled_set(const int target, const int state)
{
    int pin;
    switch (target) {
        case 0:
            pin = (1 << LED1_PIN_NUM);
            break;
        case 1:
            pin = (1 << LED2_PIN_NUM);
            break;
        case 2:
            pin = (1 << LED3_PIN_NUM);
            break;
        case 3:
            pin = (1 << LED4_PIN_NUM);
            break;
        default:
            return;
    }
    if (state) {
        LPC_GPIO2->FIOPIN &= ~pin;
    } else {
        LPC_GPIO2->FIOPIN |= pin;
    }
}

