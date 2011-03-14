#include "debled.h"
#include <LPC17xx.h>

void debled_init(void)
{
    LPC_GPIO2->FIOCLR = DEBLED1;
    LPC_GPIO2->FIOCLR = DEBLED2;
    LPC_GPIO2->FIOCLR = DEBLED3;
    LPC_GPIO2->FIOCLR = DEBLED4;

    LPC_GPIO2->FIODIR |= DEBLED1;
    LPC_GPIO2->FIODIR |= DEBLED2;
    LPC_GPIO2->FIODIR |= DEBLED3;
    LPC_GPIO2->FIODIR |= DEBLED4;
}

void debled_set(const int target, const int state)
{
    switch (target) {
        case DEBLED1:
        case DEBLED2:
        case DEBLED3:
        case DEBLED4:
            break;
        default:
            return;
    }
    if (state) {
        LPC_GPIO2->FIOPIN &= ~target;
    } else {
        LPC_GPIO2->FIOPIN |= target;
    }
}

