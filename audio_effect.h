/**
 * @file audio_effect.h
 * @brief コーデックから受け取ったデータに対して、実際の信号処理を施す。
 * @date 2011/02/19
 * @author: Takemasa Nakamura (Modified: Shinichiro Nakamura)
 */

#ifndef AUDIO_EFFECT_H
#define AUDIO_EFFECT_H

typedef struct {
    int var0;
    int var1;
    int var2;
    int var3;
} effect_param_t;

/**
 * オーディオエフェクト(スルー)を実行する。
 *
 * @param param エフェクトパラメータ。
 * @param in_left 入力データ。(L)
 * @param in_right 入力データ。(R)
 * @param out_left 出力データ。(L)
 * @param out_right 出力データ。(R)
 */
void audio_effect_through(
        const effect_param_t *param,
        const AUDIOSAMPLE *in_left,
        const AUDIOSAMPLE *in_right,
        AUDIOSAMPLE *out_left,
        AUDIOSAMPLE *out_right);

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
        AUDIOSAMPLE *out_right);

#endif

