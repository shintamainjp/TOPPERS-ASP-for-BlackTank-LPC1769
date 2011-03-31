/**
 * @file audio_effect.c
 * @brief コーデックから受け取ったデータに対して、実際の信号処理を施す。
 * @date 2011/02/19
 * @author: Takemasa Nakamura (Modified: Shinichiro Nakamura)
 */

#include "i2s_subsystem.h"
#include "audio_effect.h"

#define VOLUME_CONTROL_ENABLED 1
/**<
 * ボリューム調整を有効にする場合には真を設定する。
 */

void audio_effect_through(
        const effect_param_t *param,
        const AUDIOSAMPLE *in_left,
        const AUDIOSAMPLE *in_right,
        AUDIOSAMPLE *out_left,
        AUDIOSAMPLE *out_right)
{
#if VOLUME_CONTROL_ENABLED
    const int var0 = param->var0;
    const int var1 = param->var1;
#else
    const int var0 = 4096;
    const int var1 = 4096;
#endif
    *out_left = (*in_left) * var0;
    *out_right = (*in_right) * var0;
}

