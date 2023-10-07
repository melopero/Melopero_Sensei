#include <stdint.h>
#include "graphics.h"
#include "ST7789.h"
#include "font.h"

// #define SPI_INSTANCE              spi0
// #define MOSI                         3
// #define MISO                         4
// #define SCK                          2
// #define CS                           5
// #define DC                           7
// #define RST                          6
// #define BL                           8
// #define FREQUENCY    125 * 1000 * 1000
// #define NUM_BITS                     8
// #define HORIZONTAL                   1
// #define VERTICAL                     0
// #define DISPLAY_WIDTH              240
// #define DISPLAY_HEIGHT             320

// #define SPI_INSTANCE              spi1
// #define MOSI                        11
// define MISO                         8
// #define SCK                         10
// #define CS                           9
//  #define FREQUENCY    125 * 1000 * 1000
//  #define NUM_BITS                     8
//  #define DC                          14
//  #define RST                          0
//  #define BL                          16
//  #define TE                           1
//  #define HORIZONTAL                   0

static uint16_t frame_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];

static int x_pos = 0, y_pos = 0;

void graphics_init()
{
	ST7789_init();
}

int graphics_get_cursor_x(void)
{
	return x_pos;
}

int graphics_get_cursor_y(void)
{
	return y_pos;
}

void graphics_set_cursor_x(uint16_t x)
{
	x_pos = x;
}

void graphics_set_cursor_y(uint16_t y)
{
	y_pos = y;
}

uint16_t graphics_RGB_to_16(uint8_t red, uint8_t green, uint8_t blue)
{
	uint16_t color = (uint16_t)(red & 0xF8) << 8 | (uint16_t)(green & 0xFC) << 3 | blue >> 3;
	return color >> 8 & 0x00FF | color << 8;
}

void graphics_present_framebuffer(void)
{
	ST7789_wait_VSYNC();
	ST7789_push_framebuffer(frame_buffer);
}

void graphics_clear_framebuffer(uint8_t red, uint8_t green, uint8_t blue)
{
	for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++)
		frame_buffer[i] = graphics_RGB_to_16(red, green, blue);
}

void graphics_draw_pixel_framebuffer(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue)
{
	if (x < 0 || x >= DISPLAY_WIDTH || y < 0 || y >= DISPLAY_HEIGHT)
		return;
		
	frame_buffer[y * DISPLAY_WIDTH + x] = graphics_RGB_to_16(red, green, blue);
}

void graphics_draw_line_framebuffer(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t red, uint8_t green, uint8_t blue)
{
	/* Bresenham's line drawing algorithm */

	if (x2 < x1) // swap points
	{
		int temp = x1;
		x1 = x2;
		x2 = temp;
		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	int dx = x2 - x1;
	int dy = y2 - y1;
	int error = 0;

	if (dy >= 0) // first quadrant
	{
		if (dy <= dx) // not steep
		{
			for (int x = x1, y = y1; x <= x2; x++)
			{
				graphics_draw_pixel_framebuffer(x, y, red, green, blue);
				if (2 * (error + dy) < dx)
					error = error + dy;
				else
				{
					y = y + 1;
					error = error + dy - dx;
				}
			}
		}
		else // steep
		{
			for (int y = y1, x = x1; y <= y2; y++)
			{
				graphics_draw_pixel_framebuffer(x, y, red, green, blue);
				if (2 * (error + dx) < dy)
					error = error + dx;
				else
				{
					x = x + 1;
					error = error + dx - dy;
				}
			}
		}
	}
	else // fourth quadrant
	{
		if (-dy <= dx) // not steep
		{
			for (int x = x1, y = y1; x <= x2; x++)
			{
				graphics_draw_pixel_framebuffer(x, y, red, green, blue);
				if (2 * (error + dy) > -dx)
					error = error + dy;
				else
				{
					y = y - 1;
					error = error + dy + dx;
				}
			}
		}
		else // steep
		{
			for (int y = y1, x = x1; y >= y2; y--)
			{
				graphics_draw_pixel_framebuffer(x, y, red, green, blue);
				if (2 * (error - dx) > dy)
					error = error - dx;
				else
				{
					x = x + 1;
					error = error - dx - dy;
				}
			}
		}
	}
}

void graphics_draw_rect_framebuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue)
{
	graphics_draw_line_framebuffer(x, y, x + width - 1, y, red, green, blue);
	graphics_draw_line_framebuffer(x + width - 1, y, x + width - 1, y + height - 1, red, green, blue);
	graphics_draw_line_framebuffer(x + width - 1, y + height - 1, x, y + height -1, red, green, blue);
	graphics_draw_line_framebuffer(x, y + height -1, x, y, red, green, blue);
}

void graphics_draw_fill_rect_framebuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue)
{
	for (int i = y; i < y + height; i++)
		for (int j = x; j < x + width; j++)
			frame_buffer[i * DISPLAY_WIDTH + j] = graphics_RGB_to_16(red, green, blue);
}

void graphics_draw_sprite_framebuffer(uint16_t const sprite[], int x, int y, uint16_t width, uint16_t height, uint16_t transparency)
{
	for (int i = y; i < y + height; i++)
		for (int j = x; j < x + width; j++)
		{
			if (sprite[(i - y) * width + (j - x)] == transparency)
				continue;

			if (j < 0 || j >= DISPLAY_WIDTH || i < 0 || i >= DISPLAY_HEIGHT)
				continue;

			frame_buffer[i * DISPLAY_WIDTH + j] = sprite[(i - y) * width + (j - x)];
		}
}

void graphics_draw_char_framebuffer(uint16_t x, uint16_t y, char c, uint8_t red, uint8_t green, uint8_t blue)
{
	if (c < 32 || c > 125)
		c = '?';
	for (int h = 0; h < CHAR_HEIGHT; h++)
		for (int w = 0; w < CHAR_WIDTH; w++)
			if (font[c - 32][w + h / 8 * CHAR_WIDTH] & 1U << h % 8)
				graphics_draw_pixel_framebuffer(x + w, y + h, red, green, blue);
}

void graphics_print_framebuffer(const char *string, uint8_t red, uint8_t green, uint8_t blue)
{
	while (*string != '\0')
	{
		if (*string == '\n' || *string == '\r')
		{
			x_pos = 0;
			y_pos += CHAR_HEIGHT;
			string++;
			if (y_pos + CHAR_HEIGHT - 1 > DISPLAY_HEIGHT - 1)
			{
				graphics_clear_framebuffer(0x00, 0x00, 0x00);
				x_pos = 0;
				y_pos = 0;
			}
		}
		else
		{
			if (x_pos + CHAR_WIDTH - 1 > DISPLAY_WIDTH - 1)
			{
				x_pos = 0;
				y_pos += CHAR_HEIGHT;
			}
			if (y_pos + CHAR_HEIGHT - 1 > DISPLAY_HEIGHT - 1)
			{
				graphics_clear_framebuffer(0x00, 0x00, 0x00);
				x_pos = 0;
				y_pos = 0;
			}
			graphics_draw_char_framebuffer(x_pos, y_pos, *string++, red, green, blue);
			x_pos += CHAR_WIDTH + 1;
		}
	}
}

void graphics_clear_screen(uint8_t red, uint8_t green, uint8_t blue)
{
	ST7789_clear_screen(red, green, blue);
}

void graphics_draw_pixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue)
{
	ST7789_draw_pixel(x, y, red, green, blue);
}

void graphics_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t red, uint8_t green, uint8_t blue)
{
	/* Bresenham's line drawing algorithm */

	if (x2 < x1) // swap points
	{
		int temp = x1;
		x1 = x2;
		x2 = temp;
		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	int dx = x2 - x1;
	int dy = y2 - y1;
	int error = 0;

	if (dy >= 0) // first quadrant
	{
		if (dy <= dx) // not steep
		{
			for (int x = x1, y = y1; x <= x2; x++)
			{
				ST7789_draw_pixel(x, y, red, green, blue);
				if (2 * (error + dy) < dx)
					error = error + dy;
				else
				{
					y = y + 1;
					error = error + dy - dx;
				}
			}
		}
		else // steep
		{
			for (int y = y1, x = x1; y <= y2; y++)
			{
				ST7789_draw_pixel(x, y, red, green, blue);
				if (2 * (error + dx) < dy)
					error = error + dx;
				else
				{
					x = x + 1;
					error = error + dx - dy;
				}
			}
		}
	}
	else // fourth quadrant
	{
		if (-dy <= dx) // not steep
		{
			for (int x = x1, y = y1; x <= x2; x++)
			{
				ST7789_draw_pixel(x, y, red, green, blue);
				if (2 * (error + dy) > -dx)
					error = error + dy;
				else
				{
					y = y - 1;
					error = error + dy + dx;
				}
			}
		}
		else // steep
		{
			for (int y = y1, x = x1; y >= y2; y--)
			{
				ST7789_draw_pixel(x, y, red, green, blue);
				if (2 * (error - dx) > dy)
					error = error - dx;
				else
				{
					x = x + 1;
					error = error - dx - dy;
				}
			}
		}
	}
}

void graphics_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue)
{
	graphics_draw_line(x, y, x + width - 1, y, red, green, blue);
	graphics_draw_line(x + width - 1, y, x + width - 1, y + height - 1, red, green, blue);
	graphics_draw_line(x + width - 1, y + height - 1, x, y + height - 1, red, green, blue);
	graphics_draw_line(x, y + height - 1, x, y, red, green, blue);
}

void graphics_draw_fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue)
{
	ST7789_set_window(x, y, x + width - 1, y + height - 1);

	for (int i = 0; i < width * height; i++)
		ST7789_push_color(red, green, blue);
}

void graphics_draw_circle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
}

void graphics_draw_fill_circle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color)
{
}

void graphics_draw_triangle(point_2D p1, point_2D p2, point_2D p3, uint8_t red, uint8_t green, uint8_t blue)
{
	graphics_draw_line(p1.x, p1.y, p2.x, p2.y, red, green, blue);
	graphics_draw_line(p2.x, p2.y, p3.x, p3.y, red, green, blue);
	graphics_draw_line(p3.x, p3.y, p1.x, p1.y, red, green, blue);
}

// void graphics_draw_sprite(uint16_t *sprite, int x, int y, int width, int height)
// {
// 	uint16_t transparency = *sprite;
// 	for (int h = 0 ; h < height ; h++)
// 		for (int w = 0 ; w < width ; w++)
// 		{
// 			if (*sprite != transparency)
// 				ST7789_draw_pixel16(x + w, y + h, *sprite);
// 			sprite++;
// 		}
// }

// void graphics_draw_image(const uint16_t *image)
// {
// 	ST7789_set_window(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1);

// 	for (uint16_t i = 0 ; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++)
// 		ST7789_push_color16(*image++);
// }

/* draw a char as an array of bytes (column major order) */
void graphics_draw_char(uint16_t x, uint16_t y, char c, uint8_t red, uint8_t green, uint8_t blue)
{
	if (c < 32 || c > 125)
		c = '?';
	for (int h = 0; h < CHAR_HEIGHT; h++)
		for (int w = 0; w < CHAR_WIDTH; w++)
			if (font[c - 32][w + h / 8 * CHAR_WIDTH] & 1U << h % 8)
				ST7789_draw_pixel(x + w, y + h, red, green, blue);
}

/* print a string of characters on screen */
void graphics_print(const char *string, uint8_t red, uint8_t green, uint8_t blue)
{
	while (*string != '\0')
	{
		if (*string == '\n' || *string == '\r')
		{
			x_pos = 0;
			y_pos += CHAR_HEIGHT;
			string++;
			if (y_pos + CHAR_HEIGHT - 1 > DISPLAY_HEIGHT - 1)
			{
				ST7789_clear_screen(0x00, 0x00, 0x00);
				x_pos = 0;
				y_pos = 0;
			}
		}
		else
		{
			if (x_pos + CHAR_WIDTH - 1 > DISPLAY_WIDTH - 1)
			{
				x_pos = 0;
				y_pos += CHAR_HEIGHT;
			}
			if (y_pos + CHAR_HEIGHT - 1 > DISPLAY_HEIGHT - 1)
			{
				ST7789_clear_screen(0x00, 0x00, 0x00);
				x_pos = 0;
				y_pos = 0;
			}
			graphics_draw_char(x_pos, y_pos, *string++, red, green, blue);
			x_pos += CHAR_WIDTH + 1;
		}
	}
}

/* print a single character on screen */
void graphics_print_char(char c, uint8_t red, uint8_t green, uint8_t blue)
{
	if (c == '\n' || c == '\r')
	{
		x_pos = 0;
		y_pos += CHAR_HEIGHT;
		if (y_pos + CHAR_HEIGHT - 1 > DISPLAY_HEIGHT - 1)
		{
			ST7789_clear_screen(0x00, 0x00, 0x00);
			x_pos = 0;
			y_pos = 0;
		}
	}
	else if (c == '\b')
	{
		x_pos -= CHAR_WIDTH - 1;
		if (x_pos < 0)
		{
			x_pos = DISPLAY_WIDTH - DISPLAY_WIDTH % CHAR_WIDTH - CHAR_WIDTH;
			y_pos -= CHAR_HEIGHT;
			graphics_draw_fill_rect(x_pos, y_pos, CHAR_WIDTH - 1, CHAR_HEIGHT, 0x00, 0x00, 0x00);
		}
	}
	else
	{
		if (x_pos + CHAR_WIDTH - 1 > DISPLAY_WIDTH - 1)
		{
			x_pos = 0;
			y_pos += CHAR_HEIGHT;
		}
		if (y_pos + CHAR_HEIGHT - 1 > DISPLAY_HEIGHT - 1)
		{
			ST7789_clear_screen(0x00, 0x00, 0x00);
			x_pos = 0;
			y_pos = 0;
		}
		graphics_draw_char(x_pos, y_pos, c, red, green, blue);
		x_pos += CHAR_WIDTH + 1;
	}
}