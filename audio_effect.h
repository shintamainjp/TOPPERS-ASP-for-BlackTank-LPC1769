/**
 * @file audio_effect.h
 * @brief コーデックから受け取ったデータに対して、実際の信号処理を施す。
 * @date 2011/02/19
 * @author: Takemasa Nakamura (Modified: Shinichiro Nakamura)
 */

#ifndef AUDIO_EFFECT_H
#define AUDIO_EFFECT_H

/**
 * @brief エフェクトに対するパラメータ。
 * @details
 * エフェクトに対するパラメータの解釈は、各エフェクト処理が行う。
 */
typedef struct {
    int var0;   /**< エフェクトパラメータ値0。(意味は各エフェクタに依存) */
    int var1;   /**< エフェクトパラメータ値1。(意味は各エフェクタに依存) */
    int var2;   /**< エフェクトパラメータ値2。(意味は各エフェクタに依存) */
    int var3;   /**< エフェクトパラメータ値3。(意味は各エフェクタに依存) */
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

/**
 * オーディオエフェクト(FIR: Finite Impulse Response)を実行する。
 *
 * @param param エフェクトパラメータ。
 * @param in_left 入力データ。(L)
 * @param in_right 入力データ。(R)
 * @param out_left 出力データ。(L)
 * @param out_right 出力データ。(R)
 */
void audio_effect_fir(
        const effect_param_t *param,
        const AUDIOSAMPLE *in_left,
        const AUDIOSAMPLE *in_right,
        AUDIOSAMPLE *out_left,
        AUDIOSAMPLE *out_right);

/**
 * オーディオエフェクト(IIR: Infinite Impulse Response)を実行する。
 *
 * @param param エフェクトパラメータ。
 * @param in_left 入力データ。(L)
 * @param in_right 入力データ。(R)
 * @param out_left 出力データ。(L)
 * @param out_right 出力データ。(R)
 */
void audio_effect_iir(
        const effect_param_t *param,
        const AUDIOSAMPLE *in_left,
        const AUDIOSAMPLE *in_right,
        AUDIOSAMPLE *out_left,
        AUDIOSAMPLE *out_right);

#endif

