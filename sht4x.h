/*

 File: 		sht4x.h
 Author:	André van Schoubroeck <andre@blaatschaap.be>
 License:	MIT


 MIT License

 Copyright (c) 2024 - 2025 André van Schoubroeck <andre@blaatschaap.be>

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

#ifndef LIB_BSSENSOR_SHT4X_H_
#define LIB_BSSENSOR_SHT4X_H_

#include <stdbool.h>
#include <stdint.h>

#include <bshal_i2cm.h>

#define SHT4X_CMD_MEASURE_HIGH_ACCURACY 0xFD
#define SHT4X_CMD_MEASURE_MEDIUM_ACCURACY 0xF6
#define SHT4X_CMD_MEASURE_LOW_ACCURACY 0xE0
#define SHT4X_CMD_READ_SERIAL 0x89
#define SHT4X_CMD_RESET 0x94
#define SHT4X_CMD_HEATER_200mW_1SEC 0x39
#define SHT4X_CMD_HEATER_200mW_100mSEC 0x32
#define SHT4X_CMD_HEATER_110mW_1SEC 0x2F
#define SHT4X_CMD_HEATER_110mW_100mSEC 0x24
#define SHT4X_CMD_HEATER_20mW_1SEC 0x1E
#define SHT4X_CMD_HEATER_20mW_100mSEC 0x15

#define SHT4X_I2C_ADDR 0x44

typedef struct {
    bshal_i2cm_instance_t *p_i2c;
    uint8_t addr;
} sht4x_t;

#pragma pack(push, 1)
typedef struct {
    int16_t temperature;
    uint8_t temp_crc;
    int16_t humidity;
    uint8_t humi_crc;
} sht4x_value_t;

int sht4x_init(sht4x_t *sht4x);
int sht4x_get_temperature_C_float(sht4x_t *sht4x, float *result);
int sht4x_get_humidity_float(sht4x_t *sht4x, float *result);

#pragma pack(pop)

#endif /* LIB_BSSENSOR_SHT4X_H_ */
