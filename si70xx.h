/*

 File: 		si70xx.h
 Author:	André van Schoubroeck <andre@blaatschaap.be>
 License:	MIT


 MIT License

 Copyright (c) 2021-2025  André van Schoubroeck <andre@blaatschaap.be>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

 */
#ifndef si70xx_H_
#define si70xx_H_

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __ACCUM_FBIT__
#include <stdfix.h>
#endif

#include "bshal_i2cm.h"

typedef struct {
    bshal_i2cm_instance_t *p_i2c;
    uint8_t addr;
} si70xx_t;

#define SI7013_DEV_ID 0x0D
#define SI7020_DEV_ID 0x14
#define SI7021_DEV_ID 0x15

int si70xx_init(si70xx_t *hcd1080);

int si70xx_get_temperature_C_float(si70xx_t *si70xx, float *result);
int si70xx_get_humidity_float(si70xx_t *si70xx, float *result);

#ifdef __ACCUM_FBIT__
int si70xx_get_temperature_C_accum(si70xx_t *si70xx, accum *result);
int si70xx_get_humidity_accum(si70xx_t *si70xx, accum *result);
#endif

#endif /* si70xx_H_ */
