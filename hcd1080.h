/*

 File: 		hcd1080.h
 Author:	André van Schoubroeck  <andre@blaatschaap.be>
 License:	MIT


 MIT License

 Copyright (c) 2021 - 2025  André van Schoubroeck <andre@blaatschaap.be>

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


#ifndef HCD1080_H_
#define HCD1080_H_

#include <inttypes.h>
#include <stdbool.h>
#include <stdfix.h>
#include <stdint.h>

#include "bshal_i2cm.h"

typedef struct {
    bshal_i2cm_instance_t *p_i2c;
    uint8_t addr;
} hcd1080_t;

#define HCD1080_REG_TEMPERATURE (0x00)
#define HCD1080_REG_HUMIDITY (0x01)
#define HCD1080_REG_CONFIGURATION (0x02)
#define HCD1080_REG_SERIAL3 (0xFB)
#define HCD1080_REG_SERIAL2 (0xFC)
#define HCD1080_REG_SERIAL1 (0xFD)
#define HCD1080_REG_MANUFACTURER_ID (0xFE)
#define HCD1080_REG_DEVICE_IO (0xFF)

#define HCD1080_VAL_MANUFACTURER_ID (0x5449)
#define HCD1080_VAL_DEVICE_IO (0x1050)

int hcd1080_get_temperature_C_float(hcd1080_t *hcd1080, float *result);
int hcd1080_get_humidity_float(hcd1080_t *hcd1080, float *result);

#ifdef __ACCUM_FBIT__
int hcd1080_get_temperature_C_accum(hcd1080_t *hcd1080, accum *result);
int hcd1080_get_humidity_accum(hcd1080_t *hcd1080, accum *result);
#endif

#endif /* HCD1080_H_ */
