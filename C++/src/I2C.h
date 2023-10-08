#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include "hardware/i2c.h"

typedef struct i2c_config_t
{
    i2c_inst_t *instance;
    uint8_t SDA;
    uint8_t SCL;
    uint frequency;

} i2c_config;

void I2C_init(i2c_config *config);
void I2C_read_reg(uint8_t address, uint8_t reg, uint8_t *dst, size_t len, bool stop);
void I2C_read(uint8_t address, uint8_t *dst, size_t len, bool stop);
void I2C_write_reg(uint8_t address, uint8_t reg, uint8_t const *src, size_t len, bool stop);
void I2C_write(uint8_t address, uint8_t const *src, size_t len, bool stop);

#endif // I2C_H