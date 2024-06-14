/*

 File: 		sht2x.c
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

// Driver for SHT2x and HTU2x Humidity/Temperature sensors

#include "sht2x.h"
#include "endian.h"


//#define CLOCK_STRETCH

int sht2x_init(sht2x_t* sht2x){
	uint8_t cmd[] =   {0b11111110}; // temp without clock stretch
	int status;
	status = bshal_i2cm_send(sht2x->p_i2c, sht2x->addr, &cmd, sizeof(cmd), false);
	if (status) return status;
	// Datasheet SHT20, page 8, 5.5 Soft Reset
	// The soft reset takes less than 15 ms.
	bshal_delay_ms(15);
	return 0;
}

int sht2x_heater(sht2x_t* sht2x, bool enable){
	uint8_t cmd[] =   {0b11100110, 1};
	cmd[1] |= enable << 1;
	int status;
	status = bshal_i2cm_send(sht2x->p_i2c, sht2x->addr, &cmd, sizeof(cmd), false);
	if (status) return status;
	return 0;
}


int sht2x_get_temperature_C_float(sht2x_t* sht2x, float * result){
#ifdef CLOCK_STRETCH
	uint8_t cmd[] = {0b11100011};	// temp with clock stretch
#else
	uint8_t cmd[] =   {0b11110011}; // temp without clock stretch
#endif

	sht2x_value_t value;

	int status;
	status = bshal_i2cm_send(sht2x->p_i2c, sht2x->addr, &cmd, sizeof(cmd), false);
	if (status) return status;
#ifndef CLOCK_STRETCH
	bshal_delay_ms(85); // 14 bit max time is 85 ms.
#endif
	status = bshal_i2cm_recv(sht2x->p_i2c, sht2x->addr, &value, sizeof(value), false);
	if (status) return status;

	*result = -48.85f + (175.72f * (float)(be16toh(value.value)) / (float)(UINT16_MAX));
	return status;
}


int sht2x_get_humidity_float(sht2x_t* sht2x, float * result){
#ifdef CLOCK_STRETCH
	uint8_t cmd[] = {0b11100101};	// temp with clock stretch
#else
	uint8_t cmd[] =   {0b11110101}; // temp without clock stretch
#endif
	sht2x_value_t value;

	int status;
	status = bshal_i2cm_send(sht2x->p_i2c, sht2x->addr, &cmd, sizeof(cmd), false);
	if (status) return status;
#ifndef CLOCK_STRETCH
	bshal_delay_ms(29); // 12 bit max time is 29 ms.
#endif
	status = bshal_i2cm_recv(sht2x->p_i2c, sht2x->addr, &value, sizeof(value), false);
	if (status) return status;

	*result = -6.0f + (125.0f * (float)(be16toh(value.value)) / (float)(UINT16_MAX));
	return status;
}

int sht2x_get_serial(sht2x_t* sht2x, uint64_t * result){
	uint8_t cmd1[] = {0xFA, 0x0F};
	uint8_t cmd2[] = {0xFC, 0xC9};
	*result = 0;
	uint8_t *data = result;
	int status;

	status = bshal_i2cm_send(sht2x->p_i2c, sht2x->addr, &cmd1, sizeof(cmd1), false);
	if (status) return status;
	status = bshal_i2cm_recv(sht2x->p_i2c, sht2x->addr, data, 4, false);
	if (status) return status;

	status = bshal_i2cm_send(sht2x->p_i2c, sht2x->addr, &cmd2, sizeof(cmd2), false);
	if (status) return status;
	status = bshal_i2cm_recv(sht2x->p_i2c, sht2x->addr, data+4, 4, false);
	if (status) return status;

	return status;
}


