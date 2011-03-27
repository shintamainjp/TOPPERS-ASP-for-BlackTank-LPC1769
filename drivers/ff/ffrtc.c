/**
 * @file ffrtc.c
 * @brief FATファイルシステムドライバ用RTCモジュールの実装。
 */

#include "ffrtc.h"
#include "lpc17xx_rtc.h"

void ffrtc_init(void)
{
    RTC_Init(LPC_RTC);
}

unsigned long get_fattime(void)
{
#if 1
    RTC_TIME_Type RTCFullTime;
    RTC_GetFullTime(LPC_RTC, &RTCFullTime);
    return ((unsigned long)(RTCFullTime.YEAR - 1980) << 25)
        | ((unsigned long)RTCFullTime.MONTH << 21)
        | ((unsigned long)RTCFullTime.DOM << 16)
        | ((unsigned long)RTCFullTime.HOUR << 11)
        | ((unsigned long)RTCFullTime.MIN << 5)
        | ((unsigned long)RTCFullTime.SEC >> 1);
#else
    /* Fixed to Jan. 1, 2010 */
    return ((unsigned long)(2010 - 1980) << 25)
        | ((unsigned long)1 << 21)
        | ((unsigned long)1 << 16)
        | ((unsigned long)0 << 11)
        | ((unsigned long)0 << 5)
        | ((unsigned long)0 >> 1);
#endif
}

