/**
 * @file audio_common.h
 * @brief オーディオに関する共通の定義。
 */

#ifndef AUDIO_COMMON_H
#define AUDIO_COMMON_H

/**
 * @brief オーディオデータ型
 * @details
 * プログラム内部で使うオーディオの基本型。
 * 左詰めの符号付き固定小数点型である。小数点はMSBのすぐ右にある。
 */
typedef int AUDIOSAMPLE;

/**
 * @brief I2S DMAバッファの長さ
 * @details
 * 一回のDMA転送に使うデータバッファの長さ。
 * AUDIOBUFSIZEがNならば、ステレオなので、N/2サンプルのデータを
 * 一回のDMAで転送することになる。
 */
#define AUDIOBUFSIZE 64

/**
 * @brief 左チャンネルインデックス
 */
#define LCH 0

/**
 * @brief 右チャンネルインデックス
 */
#define RCH 1

#endif

