/*

 File: 		sht3x.c
 Author:	André van Schoubroeck
 License:	MIT


 MIT License

 Copyright (c) 2021 - 2024  André van Schoubroeck <andre@blaatschaap.be>

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

#include "sht3x.h"
#include "endian.h"

#include <bshal_delay.h>


int sht3x_init(sht3x_t* sht3x) {
	uint8_t cmd_reset[2] = {0x30, 0xA2};
	uint8_t cmd_status[2] = {0xF3, 0x2D};
	sht3x_status_t sht3x_status;
	int status;
	status = bshal_i2cm_send(sht3x->p_i2c, sht3x->addr, &cmd_reset, sizeof(cmd_reset), false);
	if (status) return status;
	bshal_delay_ms(2);
	status = bshal_i2cm_send(sht3x->p_i2c, sht3x->addr, &cmd_status, sizeof(cmd_status), false);
	if (status) return status;
	status = bshal_i2cm_recv(sht3x->p_i2c, sht3x->addr, &sht3x_status, sizeof(sht3x_status), false);
	if (status) return status;
	return !(!sht3x_status.command_status && sht3x_status.pending_alart && sht3x_status.system_reset_detected);
}

int sht3x_get_temperature_C_float(sht3x_t* sht3x, float * result){
	uint8_t cmd[2] = {0x2c, 0x10};
	sht3x_value_t value;

	int status;
	status = bshal_i2cm_send(sht3x->p_i2c, sht3x->addr, cmd, sizeof(cmd), false);
	if (status) return status;
	status = bshal_i2cm_recv(sht3x->p_i2c, sht3x->addr, &value, sizeof(value), false);
	if (status) return status;

	*result = -45.0f + 175.0f * (float)(be16toh(value.temperature)) / (float)(UINT16_MAX - 1);
	return status;
}

#ifdef __ACCUM_FBIT__
int sht3x_get_temperature_C_accum(sht3x_t* sht3x, accum * result){
	uint8_t cmd[2] = {0x2c, 0x10};
	sht3x_value_t value;

	int status;
	status = bshal_i2cm_send(sht3x->p_i2c, sht3x->addr, cmd, sizeof(cmd), false);
	if (status) return status;
	status = bshal_i2cm_recv(sht3x->p_i2c, sht3x->addr, &value, sizeof(value), false);
	if (status) return status;

	*result = -45.0lk + 175.0lk * (long accum)(be16toh(value.temperature)) / (long accum)(UINT16_MAX - 1);

	return status;
}
#endif

int sht3x_get_humidity_float(sht3x_t* sht3x, float * result){
	uint8_t cmd[2] = {0x2c, 0x10};
	sht3x_value_t value;

	int status;
	status = bshal_i2cm_send(sht3x->p_i2c, sht3x->addr, cmd, sizeof(cmd), false);
	if (status) return status;
	status = bshal_i2cm_recv(sht3x->p_i2c, sht3x->addr, &value, sizeof(value), false);
	if (status) return status;

	*result = 100.0f * (float)(be16toh(value.humidity)) / (float)(UINT16_MAX - 1);
	return status;
}

#ifdef __ACCUM_FBIT__
int sht3x_get_humidity_accum(sht3x_t* sht3x, accum * result){
	uint8_t cmd[2] = {0x2c, 0x10};
	sht3x_value_t value;

	int status;
	status = bshal_i2cm_send(sht3x->p_i2c, sht3x->addr, cmd, sizeof(cmd), false);
	if (status) return status;
	status = bshal_i2cm_recv(sht3x->p_i2c, sht3x->addr, &value, sizeof(value), false);
	if (status) return status;

	*result = 100.0lk * (long accum)(be16toh(value.humidity)) / (long accum)(UINT16_MAX - 1);

	return status;
}
#endif
