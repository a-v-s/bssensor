/*

 File: 		as3935.c
 Author:	André van Schoubroeck  <andre@blaatschaap.be>
 License:	MIT


 MIT License

 Copyright (c) 2023 - 2025  André van Schoubroeck <andre@blaatschaap.be>

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

#include "as3935.h"

int as3935_set_wdth(as3935_t *as3935, uint8_t wdth) {
    int result;
    as3925_reg1_t r1;
    result = bshal_i2cm_recv_reg(as3935->p_i2c, as3935->addr, AS3935_REG1, &r1, 1);
    if (result)
        return result;
    r1.wdth = wdth;
    result = bshal_i2cm_send_reg(as3935->p_i2c, as3935->addr, AS3935_REG1, &r1, 1);
    return result;
}

int as3935_get_distance(as3935_t *as3935, uint8_t *distance_km) {
    as3925_reg7_t r7;
    int result = bshal_i2cm_recv_reg(as3935->p_i2c, as3935->addr, AS3935_REG7, &r7, 1);
    *distance_km = r7.distance;
    return result;
}
