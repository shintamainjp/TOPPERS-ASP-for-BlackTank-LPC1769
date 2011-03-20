/**
 * \file audio_processing.c
 * \brief コーデックから受け取ったデータに対して、実際の信号処理を施す
 * \date 2011/02/19
 * \author: Takemasa Nakamura
 */

#include "i2s_subsystem.h"
	/*左チャンネルインデックス*/
#define LCH 0
	/*右チャンネルインデックス*/
#define RCH 1

void audio_effect_through(
        AUDIOSAMPLE input[2][AUDIOBUFSIZE/2],
        AUDIOSAMPLE output[2][AUDIOBUFSIZE/2],
        int count)
{
	int i;

		/*
		 * 入力データはinput[][]に格納されている。input[LCH][]が、左チャンネル、
		 * input[RCH][]が右チャンネルである。それぞれのチャンネルにはcount個の
		 * データが格納されている。各データは2の補数符号付き左詰め形式の整数
		 * （つまり1.31形式固定小数点数）である。
		 *
		 * 出力データの形式も同じである。
		 */
	for ( i = 0; i< count; i++ )
	{
#if 0
            // 1にするとボリューム制御、0にすると単純TalkThrough
            // ボリューム0の値で音量調整する
            output[LCH][i] = (input[LCH][i]>>15)*adc_read(0);
            output[RCH][i] = (input[RCH][i]>>15)*adc_read(1);
#else
			// 単なる入出力コピー
		output[LCH][i] = input[LCH][i];
		output[RCH][i] = input[RCH][i];
#endif
	}
}
