#include "SPI.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"

static spi_config config;

void SPI_init(spi_config *config_)
{
    config = *config_;

    gpio_set_function(config.mosi, GPIO_FUNC_SPI);
    gpio_set_function(config.miso, GPIO_FUNC_SPI);
    gpio_set_function(config.sck, GPIO_FUNC_SPI);

    gpio_init(config.cs);
    gpio_set_dir(config.cs, true);
    gpio_put(config.cs, 1);

    spi_set_format(config.instance, config.num_bits, config.pol, config.pha, config.msb_first ? SPI_MSB_FIRST : SPI_LSB_FIRST);
    //spi_set_slave(config.instance, false);
    spi_init(config.instance, config.frequency);
}

void SPI_write(uint8_t *data, uint32_t len)
{
    gpio_put(config.cs, 0);
    spi_write_blocking(config.instance, data, len);
    gpio_put(config.cs, 1);
}

