/*

 File: 		ahl10.h
 Author:	André van Schoubroeck  <andre@blaatschaap.be>
 License:	MIT


 MIT License

 Copyright (c) 2024-2025  André van Schoubroeck <andre@blaatschaap.be>

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

#ifndef LIB_BSSENSOR_AHT10_H_
#define LIB_BSSENSOR_AHT10_H_

#include <inttypes.h>
#include <stdbool.h>
#include <stdfix.h>
#include <stdint.h>

#include "bshal_i2cm.h"

#define AHT10_CMD_INIT 0b11100001
#define AHT10_CMD_MEASURE 0b10101100
#define AHT10_CMD_RESET 0b10111010

#define AHT10_I2C_ADDR 0x38
typedef struct {
    bshal_i2cm_instance_t *p_i2c;
    uint8_t addr;
} aht10_t;

#pragma pack(push, 1)
typedef union {
    struct {
        unsigned int : 3;
        unsigned int calibration : 1;
        unsigned int : 1;
        unsigned int working_mode : 2;
        unsigned int busy : 1;
        // Data Big endian, and we get the
        // wrong half nibble of the byte in the middle
        // Thus manual processing is required
        // unsigned int humidity : 20;
        // unsigned int temperature : 20;
        uint8_t data[5];
        unsigned int crc : 8;
    };

} aht10_value_t;
#pragma pack(pop)

int aht10_get_temperature_C_float(aht10_t *aht10, float *result);
int aht10_get_humidity_float(aht10_t *aht10, float *result);

#endif /* LIB_BSSENSOR_AHT10_H_ */
