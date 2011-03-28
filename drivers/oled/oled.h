/**
 * @file oled.h
 * @author Shinichiro Nakamura
 * @brief OLEDドライバの定義。
 * @details
 * OLED(96x64dots)の下層ドライバ。
 * リソースに対するロックは上層で行うこと。
 * 焼付防止のような処理はアプリケーション層に委ねる。
 */

/*
 * ===============================================================
 *  Aitendo OLED device driver library. (Version 0.0.2)
 *
 *  [The product]
 *   OLED-2P-095BWNN-SPI : http://www.aitendo.co.jp/product/2099
 *   ALO-095BWNN-J9      : http://www.aitendo.co.jp/product/1449
 *
 *  [References]
 *  http://serdisplib.sourceforge.net/ser/doc/Treiber_IC-SSD1332_OLED_96x64_COLOR.pdf
 * ===============================================================
 * Copyright (c) 2010-2011 Shinichiro Nakamura
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * ===============================================================
 */

#ifndef AITENDO_OLED_H
#define AITENDO_OLED_H

typedef struct color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Color;

#define OLED_X 96
#define OLED_Y 64

/**
 * @defgroup BLACKTANK_LPC1769_DRIVER_OLED
 * @ingroup BLACKTANK_LPC1769_DRIVER
 * @{
 */

void oled_init(void);

/**
 * Draw pixel.
 *
 * @param x X.
 * @param y Y.
 * @param c Color.
 */
void oled_draw_pixel(int x, int y, Color c);

/**
 * Draw line.
 *
 * @param x1 X1.
 * @param y1 Y1.
 * @param x2 X2.
 * @param y2 Y2.
 * @param c Color.
 */
void oled_draw_line(int x1, int y1, int x2, int y2, Color c);

/**
 * Fill box.
 *
 * @param x1 X1.
 * @param y1 Y1.
 * @param x2 X2.
 * @param y2 Y2.
 * @param c1 Color1.
 * @param c2 Color2.
 */
void oled_fill_box(int x1, int y1, int x2, int y2, Color c1, Color c2);

/**
 * Copy.
 *
 * @param x1 X1.
 * @param y1 Y1.
 * @param x2 X2.
 * @param y2 Y2.
 * @param nx X of the destination.
 * @param ny Y of the destination.
 */
void oled_copy(int x1, int y1, int x2, int y2, int nx, int ny);

/**
 * Darker.
 *
 * @param x1 X1.
 * @param y1 Y1.
 * @param x2 X2.
 * @param y2 Y2.
 */
void oled_darker(int x1, int y1, int x2, int y2);

/**
 * Clear.
 *
 * @param x1 X1.
 * @param y1 Y1.
 * @param x2 X2.
 * @param y2 Y2.
 */
void oled_clear(int x1, int y1, int x2, int y2);

/**
 * @}
 */

#endif
