#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include "ST7789.h"

typedef struct point_2D
{
	int x;
	int y;
} point_2D;

typedef struct triangle
{
	point_2D p1;
	point_2D p2;
	point_2D p3;
} triangle;

void graphics_init();

//uint16_t graphics_RGB_to_16(uint8_t red, uint8_t green, uint8_t blue);
void graphics_present_framebuffer(void);
void graphics_clear_framebuffer(uint8_t red, uint8_t green, uint8_t blue);
void graphics_draw_pixel_framebuffer(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);
void graphics_draw_line_framebuffer(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t red, uint8_t green, uint8_t blue);
void graphics_draw_rect_framebuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue);
void graphics_draw_fill_rect_framebuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue);
void graphics_draw_sprite_framebuffer(const uint16_t sprite[], int x, int y, uint16_t width, uint16_t height, uint16_t transparency);
void graphics_draw_char_framebuffer(uint16_t x, uint16_t y, char c, uint8_t red, uint8_t green, uint8_t blue);
void graphics_print_framebuffer(const char *string, uint8_t red, uint8_t green, uint8_t blue);

void graphics_clear_screen(uint8_t red, uint8_t green, uint8_t blue);
void graphics_draw_pixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);
void graphics_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t red, uint8_t green, uint8_t blue);
void graphics_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue);
void graphics_draw_fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue);
void graphics_draw_circle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
void graphics_draw_fill_circle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color);
void graphics_draw_triangle(point_2D p1, point_2D p2, point_2D p3, uint8_t red, uint8_t green, uint8_t blue);
//void graphics_draw_sprite(uint16_t sprite[],int x,int y,int width,int height);
//void graphics_draw_image(const uint16_t *image);
int graphics_get_cursor_x(void);
int graphics_get_cursor_y(void);
void graphics_set_cursor_x(uint16_t x);
void graphics_set_cursor_y(uint16_t y);
void graphics_draw_char(uint16_t x, uint16_t y, char c, uint8_t red, uint8_t green, uint8_t blue);
void graphics_print(const char *string, uint8_t red, uint8_t green, uint8_t blue);
void graphics_print_char(char c, uint8_t red, uint8_t green, uint8_t blue);

#endif  // GRAPHICS_H