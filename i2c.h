#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void i2c_init (void);
void i2c_start(void);
void i2c_write (uint8_t data);
void i2c_address (uint8_t Address);
void i2c_stop (void);
void i2c_writemulti (uint8_t *data, uint8_t size);
void i2c_read (uint8_t Address, uint8_t *buffer, uint8_t size);

#endif

