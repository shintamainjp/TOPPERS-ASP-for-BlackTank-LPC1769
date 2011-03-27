/**
 * \file audio_effect.c
 * \brief コーデックから受け取ったデータに対して、実際の信号処理を施す
 * \date 2011/02/19
 * \author: Takemasa Nakamura
 */

#include "i2s_subsystem.h"
#include "audio_effect.h"

#define VOLUME_CONTROL_ENABLED 1

void audio_effect_through(
        effect_param_t *param,
        AUDIOSAMPLE input[2][AUDIOBUFSIZE / 2],
        AUDIOSAMPLE output[2][AUDIOBUFSIZE / 2],
        int count)
{
    int i;

    /*
     * 入力データはinput[][]に格納されている。
     * input[LCH][]が、左チャンネル、input[RCH][]が右チャンネルである。
     * それぞれのチャンネルにはcount個のデータが格納されている。
     * 各データは2の補数符号付き左詰め形式の整数
     * （つまり1.31形式固定小数点数）である。
     * 出力データの形式も同じである。
     */
#if VOLUME_CONTROL_ENABLED
    const int var0 = param->var0;
    const int var1 = param->var1;
#endif
    for (i = 0; i < count; i++)
    {
#if VOLUME_CONTROL_ENABLED
        // 1にするとボリューム制御、0にすると単純TalkThrough
        // ボリューム0の値で音量調整する
        output[LCH][i] = (input[LCH][i] >> 10) * var0;
        output[RCH][i] = (input[RCH][i] >> 10) * var1;
#else
        // 単なる入出力コピー
        output[LCH][i] = input[LCH][i];
        output[RCH][i] = input[RCH][i];
#endif
    }
}

