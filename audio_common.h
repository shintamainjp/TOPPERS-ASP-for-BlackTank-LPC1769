#ifndef AUDIO_COMMON_H
#define AUDIO_COMMON_H

/**
 * \brief オーディオデータ型
 * \details
 * プログラム内部で使うオーディオの基本型。左詰めの符号付き固定小数点型である。小数点はMSBの
 * すぐ右にある。
 */
typedef int AUDIOSAMPLE;

/**
 * \brief I2S DMAバッファの長さ
 * \details
 * 一回のDMA転送に使うデータバッファの長さ。AUDIOBUFSIZEがNならば、ステレオなので、
 * N/2サンプルのデータを一回のDMAで転送することになる。
 */
#define	AUDIOBUFSIZE	64	 /* I2Sバッファの長さ。48kHzサンプルの時、1mSにするには96を選ぶ */

#endif

