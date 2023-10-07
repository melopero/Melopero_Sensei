#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>

extern "C"
{
#include "graphics.h"
}

class Display
{
public:
    Display()
    {
        graphics_init();
    }

    void PresentFramebuffer()
    {
        graphics_present_framebuffer();
    }

    void ClearFramebuffer(uint8_t red, uint8_t green, uint8_t blue)
    {
        graphics_clear_framebuffer(red, green, blue);
    }

    void DrawPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue)
    {
        graphics_draw_pixel_framebuffer(x, y, red, green, blue);
    }

    void DrawFillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue)
    {
        graphics_draw_fill_rect_framebuffer(x, y, width, height, red, green, blue);
    }

    void GetCursor(uint16_t &x, uint16_t &y) const
    {
        x = graphics_get_cursor_x();
        y = graphics_get_cursor_y();
    }

    void SetCursor(uint16_t x, uint16_t y)
    {
        graphics_set_cursor_x(x);
        graphics_set_cursor_y(y);
    }

    void Print(const std::string &message, uint16_t red, uint16_t green, uint16_t blue)
    {
        graphics_print_framebuffer(message.c_str(), red, green, blue);
    }

private:

};

#endif  // DISPLAY_H