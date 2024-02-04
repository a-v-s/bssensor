/*

 File: 		bh1750.c
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


#include "bh1750.h"
#include "endian.h"

int bh1750_measure_ambient_light(bh1750_t * bh1750, int *lux) {
	uint8_t cmd[] = {BH1750_INSTR_ONE_H};
	uint16_t value;

	int result;
	result = bshal_i2cm_send(bh1750->p_i2c, bh1750->addr, &cmd, sizeof(cmd), false);
	if (result) return result;
	result = bshal_i2cm_recv(bh1750->p_i2c, bh1750->addr, &value, sizeof(value), false);
	if (result) return result;

	//long accum value_in_lux = be16toh(value) / 1.2k;
	int value_in_lux = ( 10 * (int) be16toh(value) ) / 12;
	*lux = value_in_lux;
	return result;
}
