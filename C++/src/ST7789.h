#ifndef ST7789_H
#define ST7789_H

#include <stdint.h>
#include "SPI.h"

#define DISPLAY_WIDTH           240
#define DISPLAY_HEIGHT          240

typedef struct ST7789_config_t
{
    //spi_config spi;
    uint8_t dc;
    uint8_t rst;
    uint8_t bl;
    uint8_t te;
    uint width;
    uint height;
    uint8_t horizontal;

} ST7789_config;

void ST7789_init();
void ST7789_set_memory_address_control(uint8_t x_mirror, uint8_t y_mirror, uint8_t xy_exchange);

void ST7789_send_command(uint8_t command, uint8_t delay);
void ST7789_send_command_with_data(uint8_t command, uint8_t *data, uint8_t len, uint8_t delay);
void ST7789_send_data(uint8_t data);
void ST7789_send_data_buffer(uint8_t *data, uint32_t len);

void ST7789_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ST7789_clear_screen(uint8_t red, uint8_t green, uint8_t blue);
void ST7789_push_color(uint8_t red, uint8_t green, uint8_t blue);
void ST7789_draw_pixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);
void ST7789_push_framebuffer(uint16_t *framebuffer);

void ST7789_wait_VSYNC();

#endif  // ST7789_H

