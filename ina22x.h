
/*

 File: 		ina22x.h
 Author:	André van Schoubroeck  <andre@blaatschaap.be>
 License:	MIT


 MIT License

 Copyright (c) 2025  André van Schoubroeck <andre@blaatschaap.be>

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
#pragma once

#include <stdint.h>

#include "bshal_i2cm.h"

typedef enum {
    ina22x_variant_unidentified = 0,

    ina22x_variant_202 = 202,
    ina22x_variant_220 = 220,
    ina22x_variant_226 = 226,
    ina22x_variant_230 = 230,
    ina22x_variant_3221 = 3221,

    ina22x_variant_unknown = -1,
} ina22x_variant_t;

typedef struct {
    bshal_i2cm_instance_t *p_i2c;
    uint8_t addr;
    ina22x_variant_t variant;
} ina22x_t;

#pragma pack(push, 1)

typedef union {
    struct {
        unsigned int mode : 3;
        unsigned int : 12;
        unsigned int reset : 1;
    };
    struct {
        unsigned int : 3;
        unsigned int sadc : 4;
        unsigned int badc : 4;
        unsigned int pg : 2;
        unsigned int brng : 1;
    } ina220;
    struct {
        unsigned int : 3;
        unsigned int vshct : 3;
        unsigned int vbusct : 3;
        unsigned int avg : 3;
    } ina226;
    uint16_t as_uint16;
} ina220x_reg_control_t;

#pragma pack(pop)

#define INA22X_REG_CONTROL 0x00
#define INA220_VAL_CONTROL 0x399F

#define INA22X_REG_MANUFACTURER_ID 0xFE
#define INA22X_VAL_MANUFACTURER_TI 0x5449
#define INA22X_VAL_MANUFACTURER_AS 0x4153

#define INA22X_REG_DIE_ID 0xFF
#define INA22X_VAL_DIE_202 0x0200
#define INA22X_VAL_DIE_226 0x2260
#define INA22X_VAL_DIE_3221 0x3220

int ina22x_init(ina22x_t *ina22x);
