/*

 File: 		lm75b.c
 Author:	André van Schoubroeck
 License:	MIT


 MIT License

 Copyright (c) 2021-2024  André van Schoubroeck <andre@blaatschaap.be>

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

/*
 * 	This implements the LM75B I²C Temperature sensor, made by various
 * 	companies, such as Analog Devices, NXP, TI, UMW, and others.
 * 	As well as compatible devices such as the TITMP 175 and TMP1075.
 *
 *
 */

#include <stdint.h>

#ifdef __ACCUM_FBIT__
#include <stdfix.h>
#endif 
#include <stdlib.h>

#include "lm75b.h"
#include "endian.h"

int lm75b_get_temperature_C_float(lm75b_t *lm75b, float *result) {
	uint8_t reg = LM75B_REG_TEMP;
	int16_t value;

	int status;
	status = bshal_i2cm_send(lm75b->p_i2c, lm75b->addr, &reg, sizeof(reg),
			false);
	if (status)
		return status;
	status = bshal_i2cm_recv(lm75b->p_i2c, lm75b->addr, &value, sizeof(value),
			false);
	if (status)
		return status;

	// Incoming value is Big Endian
	value = be16toh(value);

	// Rathen then doing float multiplication,
	// note we are on soft float, this is expensive
	// We bit juggle a bit. Note the input is a form of fixed point
	// thus the multiplication is some bit shifting
	//*result = 0.00390625f * (float) value;
	union {
		struct {
			unsigned int fraction : 23;
			unsigned int exponent : 8;
			signed int sign : 1;
		};
		_Float32 value;
		uint32_t as_uint32;
	} f;
	if (value == 0) {
		f.exponent = 0;
		f.fraction = 0;
	} else {
		f.sign = value < 0;
		value = abs(value);
		int hpos;
		for (hpos = 14 ; hpos ; hpos--) {
			if ( value & (1 << hpos)) break;
		}
		f.exponent = 119 + hpos;
		f.fraction = value << (23 - hpos);
	}
	*result = f.value;


	return status;
}

#ifdef __ACCUM_FBIT__
int lm75b_get_temperature_C_accum(lm75b_t *lm75b, short accum *result) {
	uint8_t reg = LM75B_REG_TEMP;
	int16_t value;

	int status;
	status = bshal_i2cm_send(lm75b->p_i2c, lm75b->addr, &reg, sizeof(reg),
			false);
	if (status)
		return status;
	status = bshal_i2cm_recv(lm75b->p_i2c, lm75b->addr, &value, sizeof(value),
			false);
	if (status)
		return status;

	// Incoming value is Big Endian
	value = be16toh(value);

	//	*result = 0.00390625k * (accum) value;

	value /=2;
	*result = *(short accum*)(&value);

	return status;
}
#endif

uint16_t lm75b_get_die_id(lm75b_t *lm75b, uint16_t* die_id){
	uint8_t reg = TMP1075_REG_DIE;
	int16_t value;

	int status;
	status = bshal_i2cm_send(lm75b->p_i2c, lm75b->addr, &reg, sizeof(reg),
			false);
	if (status)
		return status;
	status = bshal_i2cm_recv(lm75b->p_i2c, lm75b->addr, &value, sizeof(value),
			false);
	if (status)
		return status;

	// Incoming value is Big Endian
	*die_id = be16toh(value);
	return status;
}
