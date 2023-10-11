#include "MeloperoSensei.hpp"

MeloperoSensei::MeloperoSensei()
{
    SPI1Init();
    I2C1Init();
    graphics_init();
    input_init();
    VSENEnable(true);
    audio_init();
}

MeloperoSensei::~MeloperoSensei()
{
    audio_deinit();
}

/**** graphics interface ****/

void clearScreen(uint8_t red, uint8_t green, uint8_t blue)
{
    graphics_clear_framebuffer(red, green, blue);
}

void presentScreen()
{
    graphics_present_framebuffer();
}

void drawPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue)
{
    graphics_draw_pixel_framebuffer(x, y, red, green, blue);
}

void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t red, uint8_t green, uint8_t blue)
{
    graphics_draw_line(x1, y1, x2, y2, red, green, blue);
}

void drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue)
{
    graphics_draw_rect_framebuffer(x, y, width, height, red, green, blue);
}

void drawFillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue)
{
    graphics_draw_fill_rect_framebuffer(x, y, width, height, red, green, blue);
}

void drawSprite(const uint16_t sprite[], int x, int y, uint16_t width, uint16_t height, uint16_t transparency = 0xE037 /*0x30FF00*/)
{
    graphics_draw_sprite_framebuffer(sprite, x, y, width, height, transparency);
}

void print(uint8_t posX, uint8_t posY, const char *string, uint8_t red, uint8_t green, uint8_t blue)
{
    graphics_set_cursor_x(posX);
    graphics_set_cursor_y(posY);
    graphics_print_framebuffer(string, red, green, blue);
}

/**** input interface ****/

ButtonState getButtonState(uint8_t button)
{
    return input_get_button_state(button);
}

/**** audio interface ****/

void playNote(float frequency, uint32_t duration, float volume, bool sweep_direction, float sweep_time)
{
    audio_play_note(frequency, duration, volume, sweep_direction, sweep_time);
}

/**** game loop ****/

void run()
{
    setup();

    while (true)
    {
        update();
        render();
    }
}
