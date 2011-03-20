/**
 * @file i2c_subsystem.h
 * @brief I2C制御関数群
 * @date 2011/02/19
 * @author Takemasa Nakamura
 * @details
 * 最初に @ref i2c_init 関数を使って、I2Cペリフェラルを初期化する。
 * @ref i2c_send_2bytes 関数は２バイトデータをI2Cデバイスに送る関数である。
 * この関数はアドホックな版であり、割り込みは使っていない。
 */

#ifndef I2C_SUBSYSTEM_H_
#define I2C_SUBSYSTEM_H_

/**
 * @brief ISC0の初期化を行う。
 * @details
 * I2C0にポートを割り当て、初期化する。
 * I2C動作クロックは100kHzにしているが、
 * これはI2Cの標準モードの最大クロックである。
 * LPC1768のコアクロックは100MHzと計算しているが、
 * もしコアクロックがことなるなら、再設定が必要である。
 */
void i2c_init();

/**
 * @brief I2C0からTLV320AIC23Bへデータを送信する
 * @param addr 7bitのI2Cアドレス。右揃え。
 * @param data0 バイト0のデータ
 * @param data1 バイト1のデータ
 * @details
 * 与えられたI2Cアドレスのデバイスに、2バイトデータを送る。
 * 基本的にはLPC1768のユーザーズマニュアルUM10360で解説されている
 * 状態遷移のうち、成功する場合だけをなぞっている。
 * 失敗した場合にはメッセージを表示するだけで、とくに回復はしていない。
 */
void i2c_send_2bytes( int addr, int data0, int data1 );

#endif /* I2C_SUBSYSTEM_H_ */
