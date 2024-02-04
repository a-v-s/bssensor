/*

 File: 		sht3x.h
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


#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdfix.h>

#include "bshal_i2cm.h"

typedef struct {
	bshal_i2cm_instance_t * p_i2c;
	uint8_t addr;
} sht3x_t;

#define SHT3X_I2C_ADDR 			0x44

#pragma pack (push,1)
typedef struct {
	int16_t temperature;
	uint8_t temp_crc;
	int16_t humidity;
	uint8_t humi_crc;
} sht3x_value_t;

typedef union {
	struct {
		unsigned int : 2;
		unsigned int t_tracking_alart : 1;
		unsigned int rh_tracking_alart : 1;
		unsigned int : 1;
		unsigned int heater_status : 1;
		unsigned int : 1;
		unsigned int pending_alart : 1;

		unsigned int write_checksum_status : 1;
		unsigned int command_status : 1;
		unsigned int : 2;
		unsigned int system_reset_detected : 1;
		unsigned int : 3;
		uint8_t crc;
	};
	uint8_t as_uint8[3];
} sht3x_status_t;
#pragma pack (pop)


int sht3x_get_temperature_C_float(sht3x_t* sht3x, float * result);
int sht3x_get_humidity_float(sht3x_t* sht3x, float * result);

#ifdef __ACCUM_FBIT__
int sht3x_get_temperature_C_accum(sht3x_t* sht3x, accum * result);
int sht3x_get_humidity_accum(sht3x_t* sht3x, accum * result);
accum sht3x_get_temperature_C_accum_ret(sht3x_t* sht3x );
#endif





