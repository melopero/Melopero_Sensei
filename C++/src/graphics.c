#include <stdint.h>
#include <stdlib.h>
#include "graphics.h"
#include "ST7789.h"
#include "font.h"

static uint16_t *frame_buffer;

void graphics_init()
{
	ST7789_init();

	frame_buffer = (uint16_t*)malloc(DISPLAY_WIDTH * DISPLAY_HEIGHT * 2);
}

void graphics_init_mp(void *buffer)
{
	ST7789_init();

	frame_buffer = buffer;
}

void graphics_deinit()
{
	free(frame_buffer);
}

uint16_t graphics_RGB_to_16(uint8_t red, uint8_t green, uint8_t blue)
{
	uint16_t color = (uint16_t)(red & 0xF8) << 8 | (uint16_t)(green & 0xFC) << 3 | blue >> 3;
	return (color >> 8 & 0x00FF) | color << 8;
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

void graphics_draw_pixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue)
{
	if (x < 0 || x >= DISPLAY_WIDTH || y < 0 || y >= DISPLAY_HEIGHT)
		return;
		
	frame_buffer[y * DISPLAY_WIDTH + x] = graphics_RGB_to_16(red, green, blue);
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
				graphics_draw_pixel(x, y, red, green, blue);
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
				graphics_draw_pixel(x, y, red, green, blue);
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
				graphics_draw_pixel(x, y, red, green, blue);
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
				graphics_draw_pixel(x, y, red, green, blue);
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
	graphics_draw_line(x + width - 1, y + height - 1, x, y + height -1, red, green, blue);
	graphics_draw_line(x, y + height -1, x, y, red, green, blue);
}

void graphics_draw_fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue)
{
	for (int i = y; i < y + height; i++)
		for (int j = x; j < x + width; j++)
			frame_buffer[i * DISPLAY_WIDTH + j] = graphics_RGB_to_16(red, green, blue);
}

void graphics_draw_sprite(uint16_t const sprite[], int x, int y, uint16_t width, uint16_t height, uint16_t transparency)
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

static int x_pos = 0, y_pos = 0;
static const uint8_t *font_bitmap = FreeMono24pt7bBitmaps;
static const int (*glyph_defs)[6] = FreeMono24pt7bGlyphs;
static uint8_t vertical_spacing = FreeMono24pt7bVerticalSpacing;
static uint8_t text_red = 0xFF, text_green = 0xFF, text_blue = 0xFF;

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

void graphics_set_text_font(enum font_name name, enum font_size size)
{
	switch (name)
	{
		case FREE_MONO:
			switch (size)
			{
				case VERY_SMALL:
					font_bitmap = FreeMono9pt7bBitmaps;
					glyph_defs = FreeMono9pt7bGlyphs;
					vertical_spacing = FreeMono9pt7bVerticalSpacing;
					break;

				case SMALL:
					font_bitmap = FreeMono12pt7bBitmaps;
					glyph_defs = FreeMono12pt7bGlyphs;
					vertical_spacing = FreeMono12pt7bVerticalSpacing;
					break;

				case REGULAR:
					font_bitmap = FreeMono18pt7bBitmaps;
					glyph_defs = FreeMono18pt7bGlyphs;
					vertical_spacing = FreeMono18pt7bVerticalSpacing;
					break;

				case BIG:
					font_bitmap = FreeMono24pt7bBitmaps;
					glyph_defs = FreeMono24pt7bGlyphs;
					vertical_spacing = FreeMono24pt7bVerticalSpacing;
					break;
			}
			break;

		case FREE_MONO_BOLD:
			switch (size)
			{
				case VERY_SMALL:
					font_bitmap = FreeMonoBold9pt7bBitmaps;
					glyph_defs = FreeMonoBold9pt7bGlyphs;
					vertical_spacing = FreeMonoBold9pt7bVerticalSpacing;
					break;

				case SMALL:
					font_bitmap = FreeMonoBold12pt7bBitmaps;
					glyph_defs = FreeMonoBold12pt7bGlyphs;
					vertical_spacing = FreeMonoBold12pt7bVerticalSpacing;
					break;

				case REGULAR:
					font_bitmap = FreeMonoBold18pt7bBitmaps;
					glyph_defs = FreeMonoBold18pt7bGlyphs;
					vertical_spacing = FreeMonoBold18pt7bVerticalSpacing;
					break;

				case BIG:
					font_bitmap = FreeMonoBold24pt7bBitmaps;
					glyph_defs = FreeMonoBold24pt7bGlyphs;
					vertical_spacing = FreeMonoBold24pt7bVerticalSpacing;
					break;
			}
			break;

		case FREE_SANS:
			switch (size)
			{
				case VERY_SMALL:
					font_bitmap = FreeSans9pt7bBitmaps;
					glyph_defs = FreeSans9pt7bGlyphs;
					vertical_spacing = FreeSans9pt7bVerticalSpacing;
					break;

				case SMALL:
					font_bitmap = FreeSans12pt7bBitmaps;
					glyph_defs = FreeSans12pt7bGlyphs;
					vertical_spacing = FreeSans12pt7bVerticalSpacing;
					break;

				case REGULAR:
					font_bitmap = FreeSans18pt7bBitmaps;
					glyph_defs = FreeSans18pt7bGlyphs;
					vertical_spacing = FreeSans18pt7bVerticalSpacing;
					break;

				case BIG:
					font_bitmap = FreeSans24pt7bBitmaps;
					glyph_defs = FreeSans24pt7bGlyphs;
					vertical_spacing = FreeSans24pt7bVerticalSpacing;
					break;
			}
			break;

		case FREE_SANS_BOLD:
			switch (size)
			{
				case VERY_SMALL:
					font_bitmap = FreeSansBold9pt7bBitmaps;
					glyph_defs = FreeSansBold9pt7bGlyphs;
					vertical_spacing = FreeSansBold9pt7bVerticalSpacing;
					break;

				case SMALL:
					font_bitmap = FreeSansBold12pt7bBitmaps;
					glyph_defs = FreeSansBold12pt7bGlyphs;
					vertical_spacing = FreeSansBold12pt7bVerticalSpacing;
					break;

				case REGULAR:
					font_bitmap = FreeSansBold18pt7bBitmaps;
					glyph_defs = FreeSansBold18pt7bGlyphs;
					vertical_spacing = FreeSansBold18pt7bVerticalSpacing;
					break;

				case BIG:
					font_bitmap = FreeSansBold24pt7bBitmaps;
					glyph_defs = FreeSansBold24pt7bGlyphs;
					vertical_spacing = FreeSansBold24pt7bVerticalSpacing;
					break;
			}
			break;
	}
}

void graphics_set_text_color(uint8_t red, uint8_t green, uint8_t blue)
{
	text_red = red;
	text_green = green;
	text_blue = blue;
}

void graphics_draw_char(uint16_t x, uint16_t y, char c)
{
	const int *glyph_def = glyph_defs[c - 32];
	int index = glyph_def[0];
	int w = glyph_def[1];
	int h = glyph_def[2];
	int offset_x = glyph_def[4];
	int offset_y = glyph_def[5];

	//int byte_length = glyph_defs[c - 32 + 1][0] - *glyph_def;

	const uint8_t *character_bitmap = &font_bitmap[index];

	for (int j = 0; j < h; j++)
		for (int k = 0; k < w; k++)
		{
			const uint8_t *bitmap = character_bitmap + (k + j * w) / 8;
			if (*bitmap & 1 << (7 - (k + j * w) % 8))
				graphics_draw_pixel(x + k + offset_x, y + j + offset_y, text_red, text_green, text_blue);   
		}
}

void graphics_print(const char *string)
{
	while (*string != '\0')
	{
		uint8_t char_width = glyph_defs[*string - 32][3];

		if (*string == '\n' || *string == '\r')
		{
			x_pos = 0;
			y_pos += vertical_spacing;
			string++;
			if (y_pos + vertical_spacing - 1 > DISPLAY_HEIGHT - 1)
			{
				graphics_clear_framebuffer(0x00, 0x00, 0x00);
				x_pos = 0;
				y_pos = 0;
			}
		}
		else
		{
			if (x_pos + char_width - 1 > DISPLAY_WIDTH - 1)
			{
				x_pos = 0;
				y_pos += vertical_spacing;
			}
			if (y_pos + vertical_spacing - 1 > DISPLAY_HEIGHT - 1)
			{
				graphics_clear_framebuffer(0x00, 0x00, 0x00);
				x_pos = 0;
				y_pos = 0;
			}
			graphics_draw_char(x_pos, y_pos, *string++);
			x_pos += char_width + 1;
		}
	}
}

