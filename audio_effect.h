#ifndef AUDIO_EFFECT_H
#define AUDIO_EFFECT_H

/**
 * \brief オーディオ信号処理関数
 * \param input 入力のオーディオデータ列。LchとRchに分かれて配列になっている
 * \param output 出力のオーディオデータ列。LchとRchに分かれて配列になっている
 * \param count 入出力データオンサンプル数。count=8のとき、入出力のデータ数はステレオなのでそれぞれ16となる。
 * \details
 * ユーザー信号処理を行う関数である。入力はあらかじめLとRに分離されて関数に渡される。同様に出力は
 * LとRを分離して受け取る。配列の添字としては、Lが0、Rが1である。
 *
 * countはサンプル数を表す。
 *
 */
void audio_effect_through(
        AUDIOSAMPLE input[2][AUDIOBUFSIZE/2],
        AUDIOSAMPLE output[2][AUDIOBUFSIZE/2],
        int count);

#endif

