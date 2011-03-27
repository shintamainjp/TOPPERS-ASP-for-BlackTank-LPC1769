/**
 * @file ffrtc.h
 * @brief FATファイルシステムドライバ用RTCモジュールの定義。
 */

#ifndef FFRTC_H
#define FFRTC_H

void ffrtc_init(void);
unsigned long get_fattime(void);

#endif

