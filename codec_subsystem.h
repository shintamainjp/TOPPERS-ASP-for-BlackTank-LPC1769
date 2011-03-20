/**
 * \file codec_subsystem.h
 * \brief TLV320AIC23B操作関数
 * \date 2011/02/19
 * \author takemasa
 * \details
 * \ref codec_init 関数を呼ぶとTLV320AIC23Bの初期化を行う。その前に \ref i2c_init 関数を
 * 呼んでおくこと。
 */

#ifndef CODEC_SUBSYSTEM_H_
#define CODEC_SUBSYSTEM_H_



/**
 * \brief TLV320AIC23Bの初期化関数
 * \details
 * 初期化文字列を与えてCODECを初期化し、動作を開始させる。この関数を呼ぶ前に
 * \ref i2c_init 関数を呼んでI2Cペリフェラルを初期化しておく。
 */
void codec_init();


#endif /* CODEC_SUBSYSTEM_H_ */
