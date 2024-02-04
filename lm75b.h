/*

 File: 		lm75b.h
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

#ifndef LM75B_H_
#define LM75B_H_

#include "bshal_i2cm.h"

#ifdef __ACCUM_FBIT__
#include <stdfix.h>
#endif 

#define	LM75B_I2C_ADDR			0x48

#define LM75B_REG_TEMP	(0x00)
#define LM75B_REG_CONF	(0x01)
#define LM75B_REG_THYST	(0x02)
#define LM75B_REG_TOS	(0x03)


typedef struct {
	bshal_i2cm_instance_t * p_i2c;
	uint8_t addr;
} lm75b_t;

int lm75b_get_temperature_C_float(lm75b_t* lm75b, float*);

#ifdef __ACCUM_FBIT__
int lm75b_get_temperature_C_accum(lm75b_t* lm75b, accum*);
#endif


#endif /* LM75B_H_ */
