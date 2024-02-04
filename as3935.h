/*

 File: 		as3935.h
 Author:	André van Schoubroeck
 License:	MIT


 MIT License

 Copyright (c) 2023 - 2024  André van Schoubroeck <andre@blaatschaap.be>

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

#ifndef LIB_BSSENSOR_AS3935_H_
#define LIB_BSSENSOR_AS3935_H_

#include <bshal_i2cm.h>
#include <stdint.h>

// This device uses illegal I²C addresses


#define AS3935_I2C_ADDR0			0x00
#define AS3935_I2C_ADDR1			0x01
#define AS3935_I2C_ADDR2			0x02
#define AS3935_I2C_ADDR3			0x03

#define AS3935_I2C_ADDR				AS3935_I2C_ADDR1

#define AS3935_REG0				0x00
#define AS3935_REG1				0x01
#define AS3935_REG2				0x02
#define AS3935_REG3				0x03
#define AS3935_REG4				0x04
#define AS3935_REG5				0x05
#define AS3935_REG6				0x06
#define AS3935_REG7				0x07
#define AS3935_REG8				0x08

typedef struct {
	bshal_i2cm_instance_t * p_i2c;
	uint8_t addr;
} as3935_t;

#pragma pack (push,1)


typedef union {
	struct {
		unsigned int pwd : 1;
		unsigned int afe_gb: 5;
	};
	uint8_t as_uint8;
} as3925_reg0_t;

typedef union {
	struct {
		unsigned int wdth : 4; // default : 0001
		unsigned int nf_lev: 3;
	};
	uint8_t as_uint8;
} as3925_reg1_t;

typedef union {
	struct {
		unsigned int distance : 6;
	};
	uint8_t as_uint8;
} as3925_reg7_t;

int as3935_get_distance(as3935_t*as3935, uint8_t*distance_km);
#pragma pack(pop)


#endif /* LIB_BSSENSOR_AS3935_H_ */
