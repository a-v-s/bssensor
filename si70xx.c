/*

 File: 		si70xx.c
 Author:	André van Schoubroeck <andre@blaatschaap.be>
 License:	MIT


 MIT License

 Copyright (c) 2021-2025  André van Schoubroeck <andre@blaatschaap.be>

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

#include "si70xx.h"
#include "endian.h"

#include <bshal_delay.h>

int si70xx_get_temperature_C_float(si70xx_t *si70xx, float *result) {
    uint8_t cmd[] = {0xe3};
    int16_t value;

    int status;
    status = bshal_i2cm_send(si70xx->p_i2c, si70xx->addr, &cmd, sizeof(cmd), false);
    if (status)
        return status;
    bshal_delay_ms(15);
    status = bshal_i2cm_recv(si70xx->p_i2c, si70xx->addr, &value, sizeof(value), false);
    if (status)
        return status;

    *result = -46.85f + 175.72f * (float)(be16toh(value)) / (float)(UINT16_MAX);
    return status;
}

#ifdef __ACCUM_FBIT__
int si70xx_get_temperature_C_accum(si70xx_t *si70xx, accum *result) {
    uint8_t cmd[] = {0xf3};
    uint16_t value;

    int status;
    status = bshal_i2cm_send(si70xx->p_i2c, si70xx->addr, &cmd, sizeof(cmd), false);
    if (status)
        return status;
    bshal_delay_ms(15);
    status = bshal_i2cm_recv(si70xx->p_i2c, si70xx->addr, &value, sizeof(value), false);
    if (status)
        return status;

    *result = -46.85lk + 175.72lk * (long accum)(be16toh(value)) / (long accum)(UINT16_MAX);

    return status;
}
#endif

int si70xx_get_humidity_float(si70xx_t *si70xx, float *result) {
    uint8_t cmd[] = {0xe5};
    int16_t value;

    int status;
    status = bshal_i2cm_send(si70xx->p_i2c, si70xx->addr, &cmd, sizeof(cmd), false);
    if (status)
        return status;
    bshal_delay_ms(15);
    status = bshal_i2cm_recv(si70xx->p_i2c, si70xx->addr, &value, sizeof(value), false);
    if (status)
        return status;

    *result = (125.0f * (float)(be16toh(value)) / (float)(UINT16_MAX)) - 6.0f;
    return status;
}

#ifdef __ACCUM_FBIT__
int si70xx_get_humidity_accum(si70xx_t *si70xx, accum *result) {
    uint8_t cmd[] = {0xe5};
    int16_t value;

    int status;
    status = bshal_i2cm_send(si70xx->p_i2c, si70xx->addr, &cmd, sizeof(cmd), false);
    if (status)
        return status;
    bshal_delay_ms(15);
    status = bshal_i2cm_recv(si70xx->p_i2c, si70xx->addr, &value, sizeof(value), false);
    if (status)
        return status;

    *result = (125.0lk * (long accum)(be16toh(value)) / (long accum)(UINT16_MAX)) - 6.0lk;

    return status;
}
#endif

int si70xx_init(si70xx_t *hcd1080) {
    uint8_t cmd_reset[] = {0xFE};
    uint8_t cmd_get_dev_id[] = {0xFC, 0xc9};
    uint8_t dev_id;
    int status;

    status = bshal_i2cm_send(hcd1080->p_i2c, hcd1080->addr, &cmd_reset, sizeof(cmd_reset), false);
    if (status)
        return status;
    bshal_delay_ms(15);

    status = bshal_i2cm_send(hcd1080->p_i2c, hcd1080->addr, &cmd_get_dev_id, sizeof(cmd_get_dev_id), false);
    if (status)
        return status;
    status = bshal_i2cm_recv(hcd1080->p_i2c, hcd1080->addr, &dev_id, sizeof(dev_id), false);
    if (status)
        return status;

    return !(dev_id == SI7013_DEV_ID || dev_id == SI7020_DEV_ID || dev_id == SI7021_DEV_ID);
    return status;
}
