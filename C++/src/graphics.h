#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include "ST7789.h"

enum font_name
{
	FREE_MONO, FREE_MONO_BOLD, FREE_SANS, FREE_SANS_BOLD,
};

enum font_size
{
	VERY_SMALL, SMALL, REGULAR, BIG,
};

void graphics_init();

//uint16_t graphics_RGB_to_16(uint8_t red, uint8_t green, uint8_t blue);
void graphics_present_framebuffer(void);
void graphics_clear_framebuffer(uint8_t red, uint8_t green, uint8_t blue);
void graphics_draw_pixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);
void graphics_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t red, uint8_t green, uint8_t blue);
void graphics_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue);
void graphics_draw_fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue);
void graphics_draw_sprite(const uint16_t sprite[], int x, int y, uint16_t width, uint16_t height, uint16_t transparency);
int graphics_get_cursor_x(void);
int graphics_get_cursor_y(void);
void graphics_set_cursor_x(uint16_t x);
void graphics_set_cursor_y(uint16_t y);
void graphics_set_text_font(enum font_name name, enum font_size size);
void graphics_set_text_color(uint8_t red, uint8_t green, uint8_t blue);
void graphics_draw_char(uint16_t x, uint16_t y, char c);
void graphics_print(const char *string);

#endif  // GRAPHICS_H