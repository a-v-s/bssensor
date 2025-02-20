/*

 File: 		adxl345.h
 Author:	André van Schoubroeck
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

#ifndef ADXL345_H_
#define ADXL345_H_

#define ADXL_I2C_ADDR1 (0x1D)
#define ADXL_I2C_ADDR0 (0x3B)

#define ADXL_SPI_MODE (3)
#define ADXL_SPI_SPEED (5000000)

typedef int (*adxl_access_register_f)(void *transport_instance, uint8_t address, uint8_t reg, uint8_t *p_data, uint8_t length);

typedef struct {
    bshal_transport_t transport_type;
    bshal_transport_instance_t transport_instance;
    uint8_t i2c_address; // Only applicable if transport is i2c
    adxl_access_register_f read;
    adxl_access_register_f write;
} adxl345_instance_t;

int adxl345_init(adxl345_instance_t *instance);

// For I²C communication we will the ADXL345 uses "standard i2c" register access, as implemented in
// bshal_i2cm_send_reg() and bshal_i2cm_recv_reg()
// SPI devices mimicking register access are not all that standard, so write an implementation for our device
// However, keeping the function signature as similar possible to keep the implementation simple.
// Therefore we have an ignored 2nd parameter, where the I²C address would go. But keeping it there
// leaves the function parameters (other then pointer type for the transport instance) the same
int adxl345_spim_send_reg(bshal_spim_instance_t *spim_instance, uint8_t ignored, uint8_t reg, uint8_t *p_data, uint8_t length);
int adxl345_spim_recv_reg(bshal_spim_instance_t *spim_instance, uint8_t ignored, uint8_t reg, uint8_t *p_data, uint8_t length);

#endif /* ADXL345_H_ */
