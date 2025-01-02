/*

 File: 		ccs811.h
 Author:	André van Schoubroeck  <andre@blaatschaap.be>
 License:	MIT


 MIT License

 Copyright (c) 2021 - 2025  André van Schoubroeck <andre@blaatschaap.be>

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

#ifndef CCS811_H_
#define CCS811_H_

#include "bshal_i2cm.h"

#define CCS811_ADDR 0x5A

typedef struct {
    bshal_i2cm_instance_t *p_i2c;
    uint8_t addr;
} ccs811_t;

#pragma pack(push, 1)
typedef union {
    struct {
        unsigned int : 2;
        unsigned int int_tresh : 1;
        unsigned int int_datardy : 1;
        unsigned int drive_mode : 3;
        unsigned int : 1;
    };
    uint8_t as_uint8;
} ccs811_meas_mode_t;

typedef struct {
    unsigned int error : 1;
    unsigned int : 2;
    unsigned int data_ready : 1;
    unsigned int app_valid : 1;
    unsigned int : 2;
    unsigned int fw_mode : 1;
} ccs811_status_t;

typedef struct {
    uint16_t eCO2;
    uint16_t eTVOC;
    ccs811_status_t status;
    uint8_t error_id;
    uint16_t raw_data;
} ccs811_alg_result_data_t;
#pragma pack(pop)

#define CCS811_I2C_ADDR1 0x5A
#define CCS811_I2C_ADDR2 0x5B

#define CCS811_REG_STATUS 0x00
#define CCS811_REG_MEAS_MODE 0x01
#define CCS811_REG_ALG_RESULT_DATA 0x02
#define CCS811_REG_RAW_DATA 0x03
#define CCS811_REG_ENV_DATA 0x05
#define CCS811_REG_THRESHOLDS 0x10
#define CCS811_REG_BASELINE 0x11
#define CCS811_REG_HW_ID 0x20
#define CCS811_REG_HW_Version 0x21
#define CCS811_REG_FW_Boot_Version 0x23
#define CCS811_REG_FW_App_Version 0x24
#define CCS811_REG_Internal_State 0xA0
#define CCS811_REG_ERROR_ID 0xE0
#define CSS811_REG_APP_ERASE 0xF1
#define CSS811_REG_APP_DATA 0xF2
#define CSS811_REG_APP_VERIFY 0xF3
#define CSS811_REG_APP_START 0xF4
#define CCS811_REG_SW_RESET 0xFF

#define CCS811_VAL_HW_ID 0x81

int ccs811_init(ccs811_t *css811);
int css811_measure(ccs811_t *css811, uint16_t *eCO2, uint16_t *TVOC);

#endif /* CCS811_H_ */
