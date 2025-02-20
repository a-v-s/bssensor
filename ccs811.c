/*

 File: 		ccs811.c
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

#include "ccs811.h"
#include "endian.h"
#include "system.h"

int ccs811_init(ccs811_t *css811) {
    //	uint8_t sw_reset[] = { 0x11, 0xE5, 0x72, 0x8A };
    //	bshal_i2cm_send_reg(css811->p_i2c, css811->addr, CCS811_REG_SW_RESET,
    //			sw_reset, 4);

    uint8_t HW_ID = 0;
    uint8_t HW_Version = 0;
    uint16_t FW_Boot_Version = 0;
    uint16_t FW_App_Version = 0;
    bshal_i2cm_recv_reg(css811->p_i2c, css811->addr, CCS811_REG_HW_ID, &HW_ID, 1);
    bshal_i2cm_recv_reg(css811->p_i2c, css811->addr, CCS811_REG_HW_Version, &HW_Version, 1);
    bshal_i2cm_recv_reg(css811->p_i2c, css811->addr, CCS811_REG_FW_Boot_Version, &FW_Boot_Version, 2);
    bshal_i2cm_recv_reg(css811->p_i2c, css811->addr, CCS811_REG_FW_App_Version, &FW_App_Version, 2);

    ccs811_status_t status;
    bshal_i2cm_recv_reg(css811->p_i2c, css811->addr, CCS811_REG_STATUS, &status, 1);
    if (CCS811_VAL_HW_ID == HW_ID) {

        uint8_t error = 0;
        if (status.error) {
            bshal_i2cm_recv_reg(css811->p_i2c, css811->addr, CCS811_REG_ERROR_ID, &error, 1);
        }

        if (status.app_valid && ~status.fw_mode) {
            // We are in bootloader mode, the application is valid
            // We need to tell the bootloader to boot the application

            uint8_t start = CSS811_REG_APP_START;
            bshal_i2cm_send(css811->p_i2c, css811->addr, &start, 1, false);
        }

        bshal_i2cm_recv_reg(css811->p_i2c, css811->addr, CCS811_REG_STATUS, &status, 1);

        if (status.fw_mode) {
            ccs811_meas_mode_t meas_mode = {
                .drive_mode = 1,
            }; // 1 sec per meas
            bshal_i2cm_send_reg(css811->p_i2c, css811->addr, CCS811_REG_MEAS_MODE, &meas_mode, 1);
            bshal_i2cm_recv_reg(css811->p_i2c, css811->addr, CCS811_REG_STATUS, &status, 1);
            if (status.error) {
                bshal_i2cm_recv_reg(css811->p_i2c, css811->addr, CCS811_REG_ERROR_ID, &error, 1);
            }
            return 0;
        } else {
            // ERROR, not in fw_mode
            return -1;
        }
    }
    return -1;
}

int css811_measure(ccs811_t *css811, uint16_t *eCO2, uint16_t *TVOC) {

    ccs811_status_t status;
    bshal_i2cm_recv_reg(css811->p_i2c, css811->addr, CCS811_REG_STATUS, &status, 1);

    if (status.data_ready) {

        ccs811_alg_result_data_t alg_result_data;

        bshal_i2cm_recv_reg(css811->p_i2c, css811->addr, CCS811_REG_ALG_RESULT_DATA, &alg_result_data, sizeof(alg_result_data));

        // Incoming data is big endian
        // 400ppm to 8192ppm
        // 0ppb to 1187ppb

        if (eCO2)
            *eCO2 = be16toh(alg_result_data.eCO2);
        if (TVOC)
            *TVOC = be16toh(alg_result_data.eTVOC);
        uint16_t raw_data = be16toh(alg_result_data.raw_data);
        return 0;
    }
    return -1;
}
