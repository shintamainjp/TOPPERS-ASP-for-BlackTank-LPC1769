#ifndef HID_H
#define HID_H

#include <stdint.h>

void hid_init(void);
unsigned char hid_swread(unsigned char ch);
void hid_swled(unsigned char ch, unsigned char on);
uint32_t hid_volread(unsigned char ch);

#endif
