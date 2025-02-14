/*

 File: 		sht4x.c
 Author:	André van Schoubroeck <andre@blaatschaap.be>
 License:	MIT


 MIT License

 Copyright (c) 2024 - 2025  André van Schoubroeck <andre@blaatschaap.be>

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

#include "sht4x.h"
#include "endian.h"

int sht4x_init(sht4x_t *sht4x) {
	uint8_t cmd[] = { SHT4X_CMD_RESET };
	int status;
	status = bshal_i2cm_send(sht4x->p_i2c, sht4x->addr, &cmd, sizeof(cmd),
			false);
	if (status)
		return status;
	bshal_delay_ms(1);
	return 0;
}

int sht4x_get_temperature_C_float(sht4x_t *sht4x, float *result) {
	uint8_t cmd[] = { SHT4X_CMD_MEASURE_LOW_ACCURACY };
	sht4x_value_t value;

	int status;
	status = bshal_i2cm_send(sht4x->p_i2c, sht4x->addr, &cmd, sizeof(cmd),
			false);
	if (status)
		return status;
	bshal_delay_ms(2);
	status = bshal_i2cm_recv(sht4x->p_i2c, sht4x->addr, &value, sizeof(value),
			false);
	if (status)
		return status;

	*result = -45.0f
			+ 175.0f * (float) (be16toh(value.temperature))
					/ (float) (UINT16_MAX - 1);
	return status;
}

int sht4x_get_humidity_float(sht4x_t *sht4x, float *result) {
	uint8_t cmd[] = { SHT4X_CMD_MEASURE_LOW_ACCURACY };
	sht4x_value_t value;

	int status;
	status = bshal_i2cm_send(sht4x->p_i2c, sht4x->addr, &cmd, sizeof(cmd),
			false);
	if (status)
		return status;
	bshal_delay_ms(2);
	status = bshal_i2cm_recv(sht4x->p_i2c, sht4x->addr, &value, sizeof(value),
			false);
	if (status)
		return status;

	*result = (125.0f * (float) (be16toh(value.humidity))
			/ (float) (UINT16_MAX - 1)) - 6.0f;
	return status;
}
