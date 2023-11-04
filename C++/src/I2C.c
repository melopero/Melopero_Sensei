#include "I2C.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include <string.h>
#include <stdlib.h>

static i2c_config config;

void I2C_init(i2c_config *config_)
{
    config = *config_;

    gpio_set_function(config.SDA, GPIO_FUNC_I2C);
    gpio_set_function(config.SCL, GPIO_FUNC_I2C);

    gpio_pull_up(config.SDA);
    gpio_pull_up(config.SCL);

    i2c_init(config.instance, config.frequency);
}

void I2C_write(uint8_t address, uint8_t const *src, size_t len, bool stop)
{
    i2c_write_blocking(config.instance, address, src, len, !stop);
}

void I2C_read(uint8_t address, uint8_t *dst, size_t len, bool stop)
{
    i2c_read_blocking(config.instance, address, dst, len, !stop);
}

void I2C_read_reg(uint8_t address, uint8_t reg, uint8_t *dst, size_t len, bool stop)
{
    // Set register to read
    I2C_write(address, &reg, 1, false);
    // Read data from register
    I2C_read(address, dst, len, stop);
}

void I2C_write_reg(uint8_t address, uint8_t reg, uint8_t const *src, size_t len, bool stop)
{  
    uint8_t buf[len];
    buf[0] = reg;
    memcpy(buf + 1, src, len);
    I2C_write(address, buf, len + 1, stop);
    free(buf);
}