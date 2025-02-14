
/*

 File: 		ina22x.h
 Author:	André van Schoubroeck  <andre@blaatschaap.be>
 License:	MIT


 MIT License

 Copyright (c) 2025  André van Schoubroeck <andre@blaatschaap.be>

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
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "bshal_i2cm.h"

typedef enum {
    ina22x_variant_unidentified = 0,

    ina22x_variant_202 = 202,
    ina22x_variant_220 = 220,
    ina22x_variant_226 = 226,
    ina22x_variant_230 = 230,
    ina22x_variant_3221 = 3221,

    ina22x_variant_unknown = -1,
} ina22x_variant_t;

typedef struct {
    bshal_i2cm_instance_t *p_i2c;
    uint8_t addr;
    ina22x_variant_t variant;
    uint16_t  shurt_resistance_mΩ;
} ina22x_t;

#pragma pack(push, 1)

typedef union {
    uint16_t as_uint16;
    struct {
        unsigned int mode : 3;
        unsigned int : 12;
        unsigned int reset : 1;
    };
    struct {
        unsigned int : 3;
        unsigned int sadc : 4;
        unsigned int badc : 4;
        unsigned int pg : 2;
        unsigned int brng : 1;
    } ina220;
    struct {
        unsigned int : 3;
        unsigned int vshct : 3;
        unsigned int vbusct : 3;
        unsigned int avg : 3;
    } ina226;
} ina22x_reg_control_t;

typedef union {
    uint16_t as_uint16;
    struct {
        signed int shunt_voltage : 16;
    };
} ina220_reg_shunt_voltage_t;

typedef enum ina220_reg_shunt_voltage_t ina226_reg_shunt_voltage_t;

typedef union {
    uint16_t as_uint16;
    struct {
        unsigned int math_overflow : 1;
        unsigned int conversion_ready_flag : 1;
        unsigned int : 1;
        unsigned int bus_voltage : 12;
    };
} ina220_reg_bus_voltage_t;

typedef union {
    uint16_t as_uint16;
    struct {
        unsigned int bus_voltage : 15;
    };
} ina226_reg_bus_voltage_t;

typedef union {
    uint16_t as_uint16;
    struct {
        unsigned int : 3;
        unsigned int bus_voltage : 13;
    };
} ina3221_reg_bus_voltage_t;
typedef union {
    uint16_t as_uint16;
    struct {
        unsigned int alert_latch_enable : 1;    // 0
        unsigned int alert_polarity : 1;        // 1
        unsigned int math_overflow : 1;         // 2
        unsigned int conversion_ready_flag : 1; // 3
        unsigned int alert_function : 1;        // 4
        unsigned int : 5;
        unsigned int alert_conversion_ready_flag : 1; // 10
        unsigned int alert_power_limit : 1;           // 11
        unsigned int alert_bus_under_voltage : 1;     // 12
        unsigned int alert_bus_over_voltage : 1;      // 13
        unsigned int alert_shunt_under_voltage : 1;   // 14
        unsigned int alert_shunt_over_voltage : 1;    // 15
    };
} ina226_reg_mask_enable_t;

typedef union {
    uint16_t as_uint16;
    struct {
        unsigned int conversion_ready_flag : 1; // 0
        unsigned int tcf : 1;
        unsigned int pvf : 1;
        unsigned int wf : 3;
        unsigned int sf : 1;
        unsigned int cf : 3;
        unsigned int cen : 1;
        unsigned int wen : 1;
        unsigned int scc : 3;
    };
} ina3221_reg_mask_enable_t;

#pragma pack(pop)

#define INA22X_REG_CONTROL 0x00
#define INA220_VAL_CONTROL 0x399F

#define INA22X_REG_MANUFACTURER_ID 0xFE
#define INA22X_VAL_MANUFACTURER_TI 0x5449
#define INA22X_VAL_MANUFACTURER_AS 0x4153

#define INA22X_REG_DIE_ID 0xFF
#define INA22X_VAL_DIE_202 0x0200
#define INA22X_VAL_DIE_226 0x2260
#define INA22X_VAL_DIE_3221 0x3220

#define INA220_REG_SHUNT_VOLTAGE 0x01
#define INA220_REG_BUS_VOLTAGE 0x02
#define INA220_REG_POWER 0x03
#define INA220_REG_CURRENT 0x04
#define INA220_REG_CALIBRATION 0x05

#define INA226_REG_SHUNT_VOLTAGE 0x01
#define INA226_REG_BUS_VOLTAGE 0x02
#define INA226_REG_POWER 0x03
#define INA226_REG_CURRENT 0x04
#define INA226_REG_CALIBRATION 0x05
#define INA226_REG_MASK_ENABLE 0x06



#define INA3221_REG_CH1_SHUNT_VOLTAGE 0x01
#define INA3221_REG_CH1_BUS_VOLTAGE 0x02
#define INA3221_REG_CH2_SHUNT_VOLTAGE 0x03
#define INA3221_REG_CH2_BUS_VOLTAGE 0x04
#define INA3221_REG_CH3_SHUNT_VOLTAGE 0x05
#define INA3221_REG_CH3_BUS_VOLTAGE 0x06
#define INA3221_REG_MASK_ENABLE 0x0F

int ina22x_init(ina22x_t *ina22x);
int ina22x_reg_read(ina22x_t *ina22x, uint8_t reg, uint16_t *val);
int ina22x_reg_write(ina22x_t *ina22x, uint8_t reg, uint16_t val);
int ina22x_conversion_ready(ina22x_t *ina22x, bool *ready);

int ina22x_voltage_float(ina22x_t *ina22x, float *voltage_mV, unsigned channel);
int ina22x_current_float(ina22x_t *ina22x, float *current_mA, unsigned channel);
