#ifndef HID_H
#define HID_H

void hid_init(void);
unsigned char hid_swread(unsigned char ch);
void hid_swled(unsigned char ch, unsigned char on);

#endif
