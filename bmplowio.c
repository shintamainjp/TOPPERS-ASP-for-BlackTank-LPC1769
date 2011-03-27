/**
 * @file bmplowio.c
 * @author Shinichiro Nakamura
 * @brief BMPファイルを少ないメモリでI/Oできるモジュールの実装。
 */

/*
 * ===============================================================
 *  BMP Low I/O Module
 *  Version 0.0.1
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

#include "bmplowio.h"

static int fread_uint32_t(
        uint32_t *data,
        int (*func_getc)(void)) {
    uint8_t temp[4];
    uint8_t i;
    for (i = 0; i < 4; i++) {
        int c = func_getc();
        if (c < 0) {
            return -1;
        }
        temp[i] = c;
    }
    *data = ((((uint32_t)temp[0]) << 0) & 0x000000ff)
        | ((((uint32_t)temp[1]) << 8) & 0x0000ff00)
        | ((((uint32_t)temp[2]) << 16) & 0x00ff0000)
        | ((((uint32_t)temp[3]) << 24) & 0xff000000) ;
    return 0;
}

static int fread_uint16_t(
        uint16_t *data,
        int (*func_getc)(void)) {
    uint8_t temp[2];
    uint8_t i;
    for (i = 0; i < 2; i++) {
        int c = func_getc();
        if (c < 0) {
            return -1;
        }
        temp[i] = c;
    }
    *data = ((((uint16_t)temp[0]) << 0) & 0x00ff)
        | ((((uint16_t)temp[1]) << 8) & 0xff00) ;
    return 0;
}

static int fwrite_uint32_t(
        uint32_t data,
        int (*func_putc)(uint8_t c)) {
    uint8_t temp[4];
    uint8_t i;
    temp[0] = ((data) >> 0) & 0x000000ff;
    temp[1] = ((data) >> 8) & 0x000000ff;
    temp[2] = ((data) >> 16) & 0x000000ff;
    temp[3] = ((data) >> 24) & 0x000000ff;
    for (i = 0; i < 4; i++) {
        if (func_putc(temp[i]) < 0) {
            return -1;
        }
    }
    return 0;
}

static int fwrite_uint16_t(
        uint16_t data,
        int (*func_putc)(uint8_t c)) {
    uint8_t temp[2];
    uint8_t i;
    temp[0] = ((data) >> 0) & 0x00ff;
    temp[1] = ((data) >> 8) & 0x00ff;
    for (i = 0; i < 2; i++) {
        if (func_putc(temp[i]) < 0) {
            return -1;
        }
    }
    return 0;
}

int bmplowio_header_init(
        bmp_file_t* filehead,
        bmp_info_t *infohead) {
    filehead->bfType = 0;
    filehead->bfType = 0;
    filehead->bfSize = 0;
    filehead->bfReserved1 = 0;
    filehead->bfReserved2 = 0;
    filehead->bfOffBits = 0;

    infohead->biSize = 0;
    infohead->biWidth = 0;
    infohead->biHeight = 0;
    infohead->biPlanes = 0;
    infohead->biBitCount = 0;
    infohead->biCompression = 0;
    infohead->biSizeImage = 0;
    infohead->biXPelsPerMeter = 0;
    infohead->biYPelsPerMeter = 0;
    infohead->biClrUsed = 0;
    infohead->biClrImportant = 0;

    return 0;
}

int bmplowio_header_write(
        int (*func_putc)(uint8_t c),
        const bmp_file_t *filehead,
        const bmp_info_t *infohead) {
    fwrite_uint16_t(filehead->bfType, func_putc);
    fwrite_uint32_t(filehead->bfSize, func_putc);
    fwrite_uint16_t(filehead->bfReserved1, func_putc);
    fwrite_uint16_t(filehead->bfReserved2, func_putc);
    fwrite_uint32_t(filehead->bfOffBits, func_putc);

    fwrite_uint32_t(infohead->biSize, func_putc);
    fwrite_uint32_t(infohead->biWidth, func_putc);
    fwrite_uint32_t(infohead->biHeight, func_putc);
    fwrite_uint16_t(infohead->biPlanes, func_putc);
    fwrite_uint16_t(infohead->biBitCount, func_putc);
    fwrite_uint32_t(infohead->biCompression, func_putc);
    fwrite_uint32_t(infohead->biSizeImage, func_putc);
    fwrite_uint32_t(infohead->biXPelsPerMeter, func_putc);
    fwrite_uint32_t(infohead->biYPelsPerMeter, func_putc);
    fwrite_uint32_t(infohead->biClrUsed, func_putc);
    fwrite_uint32_t(infohead->biClrImportant, func_putc);

    return 0;
}

int bmplowio_palette_write(
        int (*func_putc)(uint8_t c),
        const bmp_rgbquad_t *rgbquad,
        size_t n) {
    int i;
    const bmp_rgbquad_t *p = rgbquad;
    for (i = 0; i < n; i++) {
        func_putc(p->red);
        func_putc(p->green);
        func_putc(p->blue);
        func_putc(p->reserved);
        p++;
    }
    return 0;
}

int bmplowio_image_write(
        int (*func_putc)(uint8_t c),
        const bmp_file_t *filehead,
        const bmp_info_t *infohead,
        void(*func)(int x, int y, int *r, int *g, int *b)) {
    // TODO
    return 0;
}

int bmplowio_header_read(
        int (*func_getc)(void),
        bmp_file_t *filehead,
        bmp_info_t *infohead) {
    fread_uint16_t(&filehead->bfType, func_getc);
    fread_uint32_t(&filehead->bfSize, func_getc);
    fread_uint16_t(&filehead->bfReserved1, func_getc);
    fread_uint16_t(&filehead->bfReserved2, func_getc);
    fread_uint32_t(&filehead->bfOffBits, func_getc);

    fread_uint32_t(&infohead->biSize, func_getc);
    fread_uint32_t(&infohead->biWidth, func_getc);
    fread_uint32_t(&infohead->biHeight, func_getc);
    fread_uint16_t(&infohead->biPlanes, func_getc);
    fread_uint16_t(&infohead->biBitCount, func_getc);
    fread_uint32_t(&infohead->biCompression, func_getc);
    fread_uint32_t(&infohead->biSizeImage, func_getc);
    fread_uint32_t(&infohead->biXPelsPerMeter, func_getc);
    fread_uint32_t(&infohead->biYPelsPerMeter, func_getc);
    fread_uint32_t(&infohead->biClrUsed, func_getc);
    fread_uint32_t(&infohead->biClrImportant, func_getc);

    return 0;
}

int bmplowio_palette_read(
        int (*func_getc)(void),
        bmp_rgbquad_t *rgbquad,
        size_t n) {
    int i;
    bmp_rgbquad_t *p = rgbquad;
    for (i = 0; i < n; i++) {
        p->red = func_getc();
        p->green = func_getc();
        p->blue = func_getc();
        p->reserved = func_getc();
        p++;
    }
    return 0;
}

int bmplowio_image_read(
        int (*func_getc)(void),
        const bmp_file_t *filehead,
        const bmp_info_t *infohead,
        void(*func)(int x, int y, int r, int g, int b)) {
    const int w = infohead->biWidth;
    const int h = infohead->biHeight;
    const int p = (w * infohead->biBitCount + 31) / 32 * 4
                  - (w * infohead->biBitCount + 7 ) / 8;
    int x, y;
    int i;
    for (y = 0; y < infohead->biHeight; y++) {
        for (x = 0; x < infohead->biWidth; x++) {
            if (infohead->biBitCount == 24) {
                int a, b, c;
                a = (uint8_t)func_getc();
                b = (uint8_t)func_getc();
                c = (uint8_t)func_getc();
                if (func != NULL) {
                    func(x, h - y - 1, c, b, a);
                }
            }
            // TODO
        }
        for (i = 0; i < p; i++) {
            func_getc();
        }
    }
    return 0;
}

int have_palette(const bmp_info_t *infohead) {
    if ((infohead->biBitCount == 8)
            || (infohead->biBitCount == 4)
            || (infohead->biBitCount == 2)
            || (infohead->biBitCount == 1)) {
        return 1;
    }
    return 0;
}

int calc_framebytesize(const bmp_info_t *infohead) {
    const int biBitCount = infohead->biBitCount;
    const int biWidth = infohead->biWidth;
    const int biHeight = infohead->biHeight;
    int bmp_frame_bytesize = -1;

    if (biBitCount == 32) {
        bmp_frame_bytesize = biWidth * 4 * biHeight;
    }

    if (biBitCount == 24) {
        bmp_frame_bytesize =
            ((biWidth * 3 + 3) / 4) * 4 * biHeight;
    }

    if (biBitCount == 8) {
        bmp_frame_bytesize =
            ((biWidth     + 3) / 4) * 4 * biHeight;
    }

    if (biBitCount == 4) {
        bmp_frame_bytesize =
            ((biWidth / 2 + 3) / 4) * 4 * biHeight;
    }

    if (biBitCount == 2) {
        bmp_frame_bytesize =
            ((biWidth / 4 + 3) / 4) * 4 * biHeight;
    }

    if (biBitCount == 1) {
        bmp_frame_bytesize =
            ((biWidth / 8 + 3) / 4) * 4 * biHeight;
    }

    return bmp_frame_bytesize;
}
