/*

 File: 		lm75b.c
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

#include <stdint.h>

#ifdef __ACCUM_FBIT__
#include <stdfix.h>
#endif 

#include "lm75b.h"
#include "endian.h"

int lm75b_get_temperature_C_float(lm75b_t *lm75b, float *result) {
	uint8_t reg = LM75B_REG_TEMP;
	uint16_t value;

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

	value >>= 5;
	*result = 0.125f * (float) value;
	return status;
}

#ifdef __ACCUM_FBIT__
int lm75b_get_temperature_C_accum(lm75b_t *lm75b, accum *result) {
	uint8_t reg = LM75B_REG_TEMP;
	uint16_t value;

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

	value >>= 5;
	*result = 0.125k * (accum) value;
	return status;
}
#endif
