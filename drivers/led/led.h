
#ifndef LED_H
#define LED_H

#include <stdint.h>

void led_init(void);
void led_debug_write(const uint8_t target, const uint8_t on);
void led_switch_write(const uint8_t target, const uint8_t on);

#endif

