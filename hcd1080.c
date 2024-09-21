/*
 * hcd1080.c
 *
 *  Created on: 9 aug. 2021
 *      Author: andre
 */

#include "hcd1080.h"
#include "endian.h"

#include <bshal_delay.h>

int hcd1080_get_temperature_C_float(hcd1080_t *hcd1080, float *result) {
	uint8_t cmd = HCD1080_REG_TEMPERATURE;
	int16_t value;

	int status;
	status = bshal_i2cm_send(hcd1080->p_i2c, hcd1080->addr, &cmd, sizeof(cmd),
			false);
	if (status)
		return status;
	bshal_delay_ms(15);
	status = bshal_i2cm_recv(hcd1080->p_i2c, hcd1080->addr, &value,
			sizeof(value), false);
	if (status)
		return status;

	*result = -40.0f + 165.0f * (float) (be16toh(value)) / (float) (UINT16_MAX);
	return status;
}
#ifdef __ACCUM_FBIT__
int hcd1080_get_temperature_C_accum(hcd1080_t *hcd1080, accum *result) {
	uint8_t cmd = HCD1080_REG_TEMPERATURE;
	uint16_t value;

	int status;
	status = bshal_i2cm_send(hcd1080->p_i2c, hcd1080->addr, &cmd, sizeof(cmd),
			false);
	if (status)
		return status;
	bshal_delay_ms(7);
	status = bshal_i2cm_recv(hcd1080->p_i2c, hcd1080->addr, &value,
			sizeof(value), false);
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
	status = bshal_i2cm_send(hcd1080->p_i2c, hcd1080->addr, &cmd, sizeof(cmd),
			false);
	if (status)
		return status;
	bshal_delay_ms(15);
	status = bshal_i2cm_recv(hcd1080->p_i2c, hcd1080->addr, &value,
			sizeof(value), false);
	if (status)
		return status;

	*result = (100.0f * (float) (be16toh(value)) / (float) (UINT16_MAX));
	return status;
}
#ifdef __ACCUM_FBIT__
int hcd1080_get_humidity_accum(hcd1080_t *hcd1080, accum *result) {
	uint8_t cmd = HCD1080_REG_HUMIDITY;
	uint16_t value;

	int status;
	status = bshal_i2cm_send(hcd1080->p_i2c, hcd1080->addr, &cmd, sizeof(cmd),
			false);
	if (status)
		return status;
	bshal_delay_ms(7);
	status = bshal_i2cm_recv(hcd1080->p_i2c, hcd1080->addr, &value,
			sizeof(value), false);
	if (status)
		return status;

	*result = (100.0lk * (long accum)(be16toh(value)) / (long accum)(UINT16_MAX));

	return status;
}
#endif
int hcd1080_init(hcd1080_t *hcd1080) {
	int status;
	uint8_t cmd_reset[] = {HCD1080_REG_CONFIGURATION, 0x80, 0x00};
	status = bshal_i2cm_send(hcd1080->p_i2c, hcd1080->addr, &cmd_reset, sizeof(cmd_reset),
			false);
	if (status)
		return status;
	bshal_delay_ms(15);


	uint8_t cmd_init[] = {HCD1080_REG_CONFIGURATION, 0x10, 0x00};
	status = bshal_i2cm_send(hcd1080->p_i2c, hcd1080->addr, &cmd_init, sizeof(cmd_init),
			false);
	if (status)
		return status;


	uint16_t manuf, device;
	uint8_t cmd = HCD1080_REG_MANUFACTURER_ID;
	status = bshal_i2cm_send(hcd1080->p_i2c, hcd1080->addr, &cmd, sizeof(cmd),
			false);
	if (status)
		return status;
	bshal_delay_ms(1);
	status = bshal_i2cm_recv(hcd1080->p_i2c, hcd1080->addr, &manuf,
			sizeof(manuf), false);
	if (status)
		return status;

	cmd = HCD1080_REG_DEVICE_IO;
	status = bshal_i2cm_send(hcd1080->p_i2c, hcd1080->addr, &cmd, sizeof(cmd),
			false);
	if (status)
		return status;
	bshal_delay_ms(1);
	status = bshal_i2cm_recv(hcd1080->p_i2c, hcd1080->addr, &device,
			sizeof(device), false);
	if (status)
		return status;

	return !(be16toh(device) == HCD1080_VAL_DEVICE_IO
			&& be16toh(manuf) == HCD1080_VAL_MANUFACTURER_ID);

}
