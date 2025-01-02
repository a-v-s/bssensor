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

int ina22x_reg_read(ina22x_t *ina22x, uint8_t reg, uint16_t *val) {
	int result = bshal_i2cm_recv_reg(ina22x->p_i2c, ina22x->addr, reg, val,
			sizeof(*val));
	*val = htobe16(*val);
	return result;
}
int ina22x_reg_write(ina22x_t *ina22x, uint8_t reg, uint16_t val) {
	// The ina22x sensors work in big endian
	val = htobe16(val);
	return bshal_i2cm_send_reg(ina22x->p_i2c, ina22x->addr, reg, &val,
			sizeof(val));
}

int ina22x_init(ina22x_t *ina22x) {
	int result;
	ina22x_reg_control_t control = { .reset = 1 };
	// Initial Reset
	result = ina22x_reg_write(ina22x, INA22X_REG_CONTROL, control.as_uint16);
	if (result)
		return result;

	// We wish to detect which variant of the ina22x family we have.
	// Most members have a die id register, however, the ina220 does not.
	// To detect the ina220, we read the reset value of the control register.
	result = ina22x_reg_read(ina22x,
	INA22X_REG_CONTROL, &control.as_uint16);
	if (result)
		return result;

	if (control.as_uint16 == INA220_VAL_CONTROL) {
		// The reset value corresponds with the value expected of an ina220.
		// (or 219, which is considered equal from software perspective)
		ina22x->variant = ina22x_variant_220;
	} else {
		// Otherwise, we expect a part that has Manufacturer and Die ID
		// registers implemented.
		uint16_t manufacturer, die;
		result = ina22x_reg_read(ina22x, INA22X_REG_MANUFACTURER_ID,
				&manufacturer);
		if (result)
			return result;
		result = ina22x_reg_read(ina22x, INA22X_REG_DIE_ID, &die);
		if (result)
			return result;

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

int ina22x_conversion_ready(ina22x_t *ina22x, bool *ready) {
	int result;

	// The location of the conversion register differs between the variants
	switch (ina22x->variant) {
	default:
		return -1;

	case ina22x_variant_202:
	case ina22x_variant_226: {
		ina226_reg_mask_enable_t mask_enable;
		result = ina22x_reg_read(ina22x,
		INA226_REG_MASK_ENABLE, &mask_enable.as_uint16);
		if (result)
			return result;
		*ready = mask_enable.conversion_ready_flag;

		// Flag is cleared by reading the mask enable register
		// which we just did
	}
		break;
	case ina22x_variant_220: {
		ina220_reg_bus_voltage_t bus_voltage;
		result = ina22x_reg_read(ina22x,
		INA220_REG_BUS_VOLTAGE, &bus_voltage.as_uint16);
		if (result)
			return result;
		*ready = bus_voltage.conversion_ready_flag;

		// flag is cleared by reading power regostter
		uint16_t power;
		result = ina22x_reg_read(ina22x,
		INA220_REG_POWER, &power);
		if (result)
			return result;
	}
		break;
	case ina22x_variant_3221: {
		ina3221_reg_mask_enable_t mask_enable;
		result = ina22x_reg_read(ina22x,
		INA3221_REG_MASK_ENABLE, &mask_enable.as_uint16);
		if (result)
			return result;
		*ready = mask_enable.conversion_ready_flag;

		// Flag is cleared by reading the mask enable register
		// which we just did
	}
		break;
	}
	return result;
}

int ina22x_bus_voltage_float(ina22x_t *ina22x, float *voltage, unsigned channel) {
	int result = 0;

	switch (ina22x->variant) {
	default:
		return -1;
	case ina22x_variant_3221: {
		// 3 channel unit
		ina3221_reg_bus_voltage_t bus_voltage = { };
		switch (channel) {
		case 0:
			result = ina22x_reg_read(ina22x,
			INA3221_REG_CH1_BUS_VOLTAGE, &bus_voltage.as_uint16);
			break;
		case 1:
			result = ina22x_reg_read(ina22x,
			INA3221_REG_CH1_BUS_VOLTAGE, &bus_voltage.as_uint16);
			break;
		case 2:
			result = ina22x_reg_read(ina22x,
			INA3221_REG_CH1_BUS_VOLTAGE, &bus_voltage.as_uint16);
			break;
		default:
			result = -1;
		}
		// LSB is 8 mV
		*voltage = bus_voltage.bus_voltage * 8.0f;
	}
		break;
	case ina22x_variant_202:
	case ina22x_variant_226: {
		if (channel)
			return -1;
		ina226_reg_bus_voltage_t bus_voltage = { };
		result = ina22x_reg_read(ina22x,
		INA226_REG_BUS_VOLTAGE, &bus_voltage.as_uint16);
		// LSB = 1.25 mV
		*voltage = bus_voltage.bus_voltage * 1.25;
	}
		break;
	case ina22x_variant_220: {
		if (channel)
			return -1;
		ina220_reg_bus_voltage_t bus_voltage = { };
		result = ina22x_reg_read(ina22x,
		INA220_REG_BUS_VOLTAGE, &bus_voltage.as_uint16);
		// LSB = 4 mV
		*voltage = bus_voltage.bus_voltage * 4.0f;
	}
		break;

	}

	return result;
}
