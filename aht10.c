/*

 File: 		ahl10.c
 Author:	André van Schoubroeck
 License:	MIT


 MIT License

 Copyright (c) 2024  André van Schoubroeck <andre@blaatschaap.be>

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

// ASAIR AHT10, AHT2x driver
// Note the AHT20, AHT21 datasheets specify additional parameters
// to the initialisation command, different for each, with no way
// to detect which chip is present documented. Implementing the AHT10
// without any parameters appears to work for the AHT20 and AHT21 as
// well. The data returns appers to be correct.
#include "aht10.h"
#include <endian.h>

int aht10_init(aht10_t *aht10) {
	char cmd = AHT10_CMD_INIT;
	int status;
	status = bshal_i2cm_send(aht10->p_i2c, aht10->addr, &cmd, sizeof(cmd),
			false);
	return status;
}

int aht10_get_temperature_C_float(aht10_t *aht10, float *result) {

	aht10_value_t value;
	char cmd = AHT10_CMD_MEASURE;
	int status;
	status = bshal_i2cm_send(aht10->p_i2c, aht10->addr, &cmd, sizeof(cmd),
			false);
	if (status)
		return status;

// 	Waiting for the data to be ready, can take a while, or read old data istead?
//	value.busy= 1;
//	while (value.busy) {
//		status = bshal_i2cm_recv(aht10->p_i2c, aht10->addr, &value, sizeof(value), false);
//		if (status) return status;
//	}
	status = bshal_i2cm_recv(aht10->p_i2c, aht10->addr, &value, sizeof(value),
			false);
	if (status)
		return status;

	// Big endian data, can't use bitfields as those will take the wrong nibble
	// of the shared byte in the middle
	uint32_t hum_raw = ((value.data[0]) << 12) | (value.data[1] << 4)
			| (value.data[2] >> 4);
	uint32_t temp_raw = ((value.data[2] << 16) | (value.data[3] << 8)
			| value.data[4]) & 0xFFFFF;
	*result = (((float) (temp_raw) / (float) (1 << 20)) * 200.0f) - 50;
	return status;

}

int aht10_get_humidity_float(aht10_t *aht10, float *result) {
	aht10_value_t value;
	char cmd = AHT10_CMD_MEASURE;
	int status;
	status = bshal_i2cm_send(aht10->p_i2c, aht10->addr, &cmd, sizeof(cmd),
			false);
	if (status)
		return status;

// Waiting for the data to be ready, can take a while, or read old data istead?
//	value.busy= 1;
//	while (value.busy) {
//		status = bshal_i2cm_recv(aht10->p_i2c, aht10->addr, &value, sizeof(value), false);
//		if (status) return status;
//	}
	status = bshal_i2cm_recv(aht10->p_i2c, aht10->addr, &value, sizeof(value),
			false);
	if (status)
		return status;

	// Big endian data, can't use bitfields as those will take the wrong nibble
	// of the shared byte in the middle
	uint32_t hum_raw = ((value.data[0]) << 12) | (value.data[1] << 4)
			| (value.data[2] >> 4);
	uint32_t temp_raw = ((value.data[2] << 16) | (value.data[3] << 8)
			| value.data[4]) & 0xFFFFF;
	*result = (((float) (hum_raw) / (float) (1 << 20)) * 100.0f);
	return status;
}

