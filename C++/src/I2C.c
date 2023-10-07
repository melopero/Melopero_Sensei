#include "I2C.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

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
