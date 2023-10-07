#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "hardware/spi.h"

typedef struct spi_config_t
{
    spi_inst_t *instance;
    uint8_t mosi;
    uint8_t miso;
    uint8_t sck;
    uint8_t cs;
    uint frequency;
    uint8_t num_bits;
    spi_cpol_t pol;
    spi_cpha_t pha;
    uint8_t msb_first;

} spi_config;

void SPI_init(spi_config *config);
void SPI_write(uint8_t *data, uint32_t len);

#endif // SPI_H