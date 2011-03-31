/**
 * @file audio_effect.c
 * @brief コーデックから受け取ったデータに対して、実際の信号処理を施す。
 * @date 2011/02/19
 * @author: Takemasa Nakamura (Modified: Shinichiro Nakamura)
 */

#include "i2s_subsystem.h"
#include "audio_effect.h"

void audio_effect_through(
        const effect_param_t *param,
        const AUDIOSAMPLE *in_left,
        const AUDIOSAMPLE *in_right,
        AUDIOSAMPLE *out_left,
        AUDIOSAMPLE *out_right)
{
    const int var0 = param->var0;
    const int var1 = param->var1;
    *out_left = ((*in_left) >> 10) * var0;
    *out_right = ((*in_right) >> 10) * var1;
}

/**
 * オーディオエフェクト(ボーカルキャンセル)を実行する。
 *
 * @param param エフェクトパラメータ。
 * @param in_left 入力データ。(L)
 * @param in_right 入力データ。(R)
 * @param out_left 出力データ。(L)
 * @param out_right 出力データ。(R)
 */
void audio_effect_vocal_cancel(
        const effect_param_t *param,
        const AUDIOSAMPLE *in_left,
        const AUDIOSAMPLE *in_right,
        AUDIOSAMPLE *out_left,
        AUDIOSAMPLE *out_right)
{
    const int var0 = param->var0;
    const int var1 = param->var1;
    AUDIOSAMPLE mix = ((*in_left) / 2) + ((*in_right) / 2);
    *out_left = (mix >> 10) * var0;
    *out_right = (mix >> 10) * var1;
}

