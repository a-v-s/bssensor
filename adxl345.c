/*

 File: 		adxl345.c
 Author:	André van Schoubroeck <andre@blaatschaap.be>
 License:	MIT


 MIT License

 Copyright (c) 2022-2025  André van Schoubroeck <andre@blaatschaap.be>

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

#include "adxl345.h"

int adxl345_init(adxl345_instance_t *instance) {
    if (!instance)
        return -1;
    switch (instance->transport_type) {
    case bshal_transport_i2c:
        instance->read = (adxl_access_register_f)(bshal_i2cm_recv_reg);
        instance->write = (adxl_access_register_f)(bshal_i2cm_send_reg);
        break;
    case bshal_transport_spi:
        instance->read = (adxl_access_register_f)(adxl345_spim_recv_reg);
        instance->write = (adxl_access_register_f)(adxl345_spim_send_reg);
        break;
    default:
        return -2;
    }
}
