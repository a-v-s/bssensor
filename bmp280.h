/*

 File: 		bmp280.h
 Author:	André van Schoubroeck
 License:	MIT


 MIT License

 Copyright (c) 2021  André van Schoubroeck <andre@blaatschaap.be>

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

#ifndef __BMP_280_H__
#define __BMP_280_H__
#include "bshal_i2cm.h"

#include <stdint.h>

#pragma pack (push,1)

typedef struct {
	uint16_t t1;
	int16_t	t2;
	int16_t t3;
	uint16_t p1;
	int16_t p2;
	int16_t p3;
	int16_t p4;
	int16_t p5;
	int16_t p6;
	int16_t p7;
	int16_t p8;
	int16_t p9;
} bmp280_trimming_t;

typedef struct {
	union {
		unsigned int be :24;
		uint8_t raw[3];
	} up;
	union {
		unsigned int be :24;
		uint8_t raw[3];
	} ut;
} bmp280_measurement_t;

typedef struct {
	bshal_i2cm_instance_t * p_i2c;
	uint8_t addr;
} bmp280_t;

#pragma pack(pop)

#define BMP280_REG_TRIM	(0x88)
#define BMP280_REG_MEAS	(0xF7)

#define BMP280_I2C_ADDR1 (0x76)
#define BMP280_I2C_ADDR2 (0x77)
#define BMP280_I2C_ADDR BMP280_I2C_ADDR1


int bmp280_measure_f(bmp280_t *bmp280, float *temperature, float *pressure) ;
int bmp280_init(bmp280_t *bmp280);
#endif // __BMP_280_H__
