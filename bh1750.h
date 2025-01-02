/*

 File: 		bh1750.h
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

#ifndef BH1750_H_
#define BH1750_H_

#include <inttypes.h>
#include <stdbool.h>
#include <stdfix.h>
#include <stdint.h>

#include "bshal_i2cm.h"

typedef struct {
    bshal_i2cm_instance_t *p_i2c;
    uint8_t addr;
} bh1750_t;

#define BH1750_I2C_ADDR 0x23 // Light Sensor

#define BH1750_INSTR_POWER_DOWN 0b00000000
#define BH1750_INSTR_POWER_ON 0b00000001
#define BH1750_INSTR_RESET 0b00000111

#define BH1750_INSTR_CONT_H 0b00010000
#define BH1750_INSTR_CONT_H2 0b00010001
#define BH1750_INSTR_CONT_L 0b00010011

#define BH1750_INSTR_ONE_H 0b00100000
#define BH1750_INSTR_ONE_H2 0b00100001
#define BH1750_INSTR_ONE_L 0b00100011

int bh1750_measure_ambient_light(bh1750_t *bh1750, int *lux);

#endif /* BH1750_H_ */
