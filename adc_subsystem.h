/**
 * @file adc_subsystem.h
 * @brief 内蔵ADC制御関数
 * @date 2011/02/19
 * @author Takemasa Nakamura
 * @details
 * @ref adc_init を使ってADCを初期化する。
 * この初期化関数はADCをバーストモードに設定して変換を開始させる。
 * 変換は自動で継続するので、必要なタイミングで @ref adc_read() を
 * 使って値を読めばよい。
 */

#ifndef ADC_SUBSYSTEM_H_
#define ADC_SUBSYSTEM_H_

/**
 * @brief ADCの初期化を行う
 * @details
 * LPC1768内蔵ADCの初期化を行い、クロックを5MHzに設定している。
 * 5MHzに深い意味はない。
 * 使用しているADCは0-3のみで、これはTakachihoのボード構成にそっている。
 * 初期化後は、バースとモードで自動的に変換を続ける。
 */
void adc_init();

/**
 * @brief ADCから値を読み込んでかえす
 * @param ch ADCチャンネル番号 0から7
 * @return　左詰めの16bit符号なしのADC変換値
 */
unsigned short adc_read( int ch);

#endif /* ADC_SUBSYSTEM_H_ */
