/*

 File: 		ina22x.c
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

#include "ina22x.h"
#include "endian.h"

int ina22x_init(ina22x_t *ina22x) {
    int result;
    ina220x_reg_control_t control = {.reset = 1};
    uint16_t temp;

    // The ina22x sensors work in big endian
    temp = htobe16(control.as_uint16);
    // Start with issuing a reset
    result = bshal_i2cm_send_reg(ina22x->p_i2c, ina22x->addr, INA22X_REG_CONTROL, &temp, sizeof(temp));
    if (result)
        return result;

    // We wish to detect which variant of the ina22x family we have.
    // Most members have a die id register, however, the ina220 does not.
    // To detect the ina220, we read the reset value of the control register.
    result = bshal_i2cm_recv_reg(ina22x->p_i2c, ina22x->addr, INA22X_REG_CONTROL, &temp, sizeof(temp));
    if (result)
        return result;
    control.as_uint16 = htobe16(temp);
    if (control.as_uint16 == INA220_VAL_CONTROL) {
        // The reset value corresponds with the value expected of an ina220.
        // (or 219, which is considered equal from software perspective)
        ina22x->variant = ina22x_variant_220;
    } else {
        // Otherwise, we expect a part that has Manufacturer and Die ID
        // registers implemented.
        uint16_t manufacturer, die;

        result = bshal_i2cm_recv_reg(ina22x->p_i2c, ina22x->addr, INA22X_REG_MANUFACTURER_ID, &manufacturer, sizeof(manufacturer));
        result = bshal_i2cm_recv_reg(ina22x->p_i2c, ina22x->addr, INA22X_REG_DIE_ID, &die, sizeof(die));
        manufacturer = be16toh(manufacturer);
        die = be16toh(die);

        switch (manufacturer) {
        default:
            // unknown
            ina22x->variant = ina22x_variant_unknown;
            break;
        case INA22X_VAL_MANUFACTURER_TI:
            // TI part
            switch (die) {
            default:
                // unknown
                ina22x->variant = ina22x_variant_unknown;
                break;
            case INA22X_VAL_DIE_226:
                ina22x->variant = ina22x_variant_226;
                break;
            case INA22X_VAL_DIE_3221:
                ina22x->variant = ina22x_variant_3221;
                break;
            }
            break;
        case INA22X_VAL_MANUFACTURER_AS:
            // For AnalogySemi parts, the 4 least significant bits are
            // reserved and should be ignored for identification
            die &= 0xFFF0;
            switch (die) {
            default:
                // unknown
                ina22x->variant = ina22x_variant_unknown;
                break;
            case INA22X_VAL_DIE_202:
                // At this time the only supported AnalogySemi variant is CSD202
                ina22x->variant = ina22x_variant_202;
                break;
            }
        }
    }

    return ina22x->variant != ina22x_variant_unknown;
}
