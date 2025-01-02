/*

 File: 		hcd1080.c
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

#include "hcd1080.h"
#include "endian.h"

#include <bshal_delay.h>

int hcd1080_get_temperature_C_float(hcd1080_t *hcd1080, float *result) {
    uint8_t cmd = HCD1080_REG_TEMPERATURE;
    int16_t value;

    int status;
    status = bshal_i2cm_send(hcd1080->p_i2c, hcd1080->addr, &cmd, sizeof(cmd), false);
    if (status)
        return status;
    bshal_delay_ms(15);
    status = bshal_i2cm_recv(hcd1080->p_i2c, hcd1080->addr, &value, sizeof(value), false);
    if (status)
        return status;

    *result = -40.0f + 165.0f * (float)(be16toh(value)) / (float)(UINT16_MAX);
    return status;
}
#ifdef __ACCUM_FBIT__
int hcd1080_get_temperature_C_accum(hcd1080_t *hcd1080, accum *result) {
    uint8_t cmd = HCD1080_REG_TEMPERATURE;
    uint16_t value;

    int status;
    status = bshal_i2cm_send(hcd1080->p_i2c, hcd1080->addr, &cmd, sizeof(cmd), false);
    if (status)
        return status;
    bshal_delay_ms(7);
    status = bshal_i2cm_recv(hcd1080->p_i2c, hcd1080->addr, &value, sizeof(value), false);
    if (status)
        return status;

    *result = -40.0 + 165.0 * (long accum)(be16toh(value)) / (long accum)(UINT16_MAX);

    return status;
}
#endif
int hcd1080_get_humidity_float(hcd1080_t *hcd1080, float *result) {
    uint8_t cmd = HCD1080_REG_HUMIDITY;
    uint16_t value;

    int status;
    status = bshal_i2cm_send(hcd1080->p_i2c, hcd1080->addr, &cmd, sizeof(cmd), false);
    if (status)
        return status;
    bshal_delay_ms(15);
    status = bshal_i2cm_recv(hcd1080->p_i2c, hcd1080->addr, &value, sizeof(value), false);
    if (status)
        return status;

    *result = (100.0f * (float)(be16toh(value)) / (float)(UINT16_MAX));
    return status;
}
#ifdef __ACCUM_FBIT__
int hcd1080_get_humidity_accum(hcd1080_t *hcd1080, accum *result) {
    uint8_t cmd = HCD1080_REG_HUMIDITY;
    uint16_t value;

    int status;
    status = bshal_i2cm_send(hcd1080->p_i2c, hcd1080->addr, &cmd, sizeof(cmd), false);
    if (status)
        return status;
    bshal_delay_ms(7);
    status = bshal_i2cm_recv(hcd1080->p_i2c, hcd1080->addr, &value, sizeof(value), false);
    if (status)
        return status;

    *result = (100.0lk * (long accum)(be16toh(value)) / (long accum)(UINT16_MAX));

    return status;
}
#endif
int hcd1080_init(hcd1080_t *hcd1080) {
    int status;
    uint8_t cmd_reset = {HCD1080_REG_CONFIGURATION, 0x80, 0x00};
    status = bshal_i2cm_send(hcd1080->p_i2c, hcd1080->addr, &cmd_reset, sizeof(cmd_reset), false);
    if (status)
        return status;
    bshal_delay_ms(15);

    uint8_t cmd_init = {HCD1080_REG_CONFIGURATION, 0x10, 0x00};
    status = bshal_i2cm_send(hcd1080->p_i2c, hcd1080->addr, &cmd_init, sizeof(cmd_init), false);
    if (status)
        return status;

    uint16_t manuf, device;
    uint8_t cmd = HCD1080_REG_MANUFACTURER_ID;
    status = bshal_i2cm_send(hcd1080->p_i2c, hcd1080->addr, &cmd, sizeof(cmd), false);
    if (status)
        return status;
    bshal_delay_ms(1);
    status = bshal_i2cm_recv(hcd1080->p_i2c, hcd1080->addr, &manuf, sizeof(manuf), false);
    if (status)
        return status;

    cmd = HCD1080_REG_DEVICE_IO;
    status = bshal_i2cm_send(hcd1080->p_i2c, hcd1080->addr, &cmd, sizeof(cmd), false);
    if (status)
        return status;
    bshal_delay_ms(1);
    status = bshal_i2cm_recv(hcd1080->p_i2c, hcd1080->addr, &device, sizeof(device), false);
    if (status)
        return status;

    return !(be16toh(device) == HCD1080_VAL_DEVICE_IO && be16toh(manuf) == HCD1080_VAL_MANUFACTURER_ID);
}
