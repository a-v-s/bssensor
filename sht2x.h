/*

 File: 		sht2x.h
 Author:	André van Schoubroeck <andre@blaatschaap.be>
 License:	MIT


 MIT License

 Copyright (c) 2021 - 2025 André van Schoubroeck <andre@blaatschaap.be>

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

#include <inttypes.h>
#include <stdbool.h>
#include <stdfix.h>
#include <stdint.h>

#include "bshal_i2cm.h"

typedef struct {
    bshal_i2cm_instance_t *p_i2c;
    uint8_t addr;
} sht2x_t;

#pragma pack(push, 1)
typedef struct {
    int value : 14;
    int status : 2;
} sht2x_value_t;
#pragma pack(pop)

int sht2x_init(sht2x_t *sht2x);
int sht2x_get_temperature_C_float(sht2x_t *sht2x, float *result);
int sht2x_get_humidity_float(sht2x_t *sht2x, float *result);
