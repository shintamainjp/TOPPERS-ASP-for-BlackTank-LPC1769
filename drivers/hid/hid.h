#ifndef HID_H
#define HID_H

#include <stdint.h>

void hid_init(void);
uint8_t hid_swread(unsigned char ch);
uint32_t hid_volread(unsigned char ch);

#endif

