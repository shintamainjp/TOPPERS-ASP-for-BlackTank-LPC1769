/**
 * @file led.h
 * @author Shinichiro Nakamura
 * @brief LEDドライバの定義。
 */

#ifndef LED_H
#define LED_H

#include <stdint.h>

/**
 * @defgroup BLACKTANK_LPC1769_DRIVER_LED
 * @ingroup BLACKTANK_LPC1769_DRIVER
 * @{
 */

void led_init(void);
void led_debug_write(const uint8_t target, const uint8_t on);
void led_switch_write(const uint8_t target, const uint8_t on);

/**
 * @}
 */

#endif

