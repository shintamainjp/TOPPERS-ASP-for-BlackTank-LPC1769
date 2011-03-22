/**
 * =============================================================================
 * Anchor - A tiny GUI framework for embedded system. (Version 0.0.1)
 * =============================================================================
 * Copyright (c) 2010 Shinichiro Nakamura (CuBeatSystems)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * =============================================================================
 */
#ifndef BMPLOWIO_H
#define BMPLOWIO_H

#include <kernel.h>
#include <t_stdlib.h>

#define BMP_FILE_MAGIC_TEXT    (('B' << 0) | ('M' << 8))

typedef struct bmp_file {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} bmp_file_t;

typedef struct bmp_info {
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} bmp_info_t;

typedef struct bmp_rgbquad {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t reserved;
} bmp_rgbquad_t;

int bmplowio_header_init(
    bmp_file_t* filehead,
    bmp_info_t *infohead);

int bmplowio_header_write(
    int (*func_putc)(uint8_t c),
    const bmp_file_t* filehead,
    const bmp_info_t *infohead);

int bmplowio_palette_write(
    int (*func_putc)(uint8_t c),
    const bmp_rgbquad_t *rgbquad,
    size_t n);

int bmplowio_image_write(
    int (*func_putc)(uint8_t c),
    const bmp_file_t *filehead,
    const bmp_info_t *infohead,
    void(*func)(int x, int y, int *r, int *g, int *b));

int bmplowio_header_read(
    int (*func_getc)(void),
    bmp_file_t *filehead,
    bmp_info_t *infohead);

int bmplowio_palette_read(
    int (*func_getc)(void),
    bmp_rgbquad_t *rgbquad,
    size_t n);

int bmplowio_image_read(
    int (*func_getc)(void),
    const bmp_file_t *filehead,
    const bmp_info_t *infohead,
    void(*func)(int x, int y, int r, int g, int b));

int have_palette(const bmp_info_t *infoh);

int calc_framebytesize(const bmp_info_t *infoh);

#endif

