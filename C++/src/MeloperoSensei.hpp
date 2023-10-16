#ifndef MELOPEROSENSEI_H
#define MELOPEROSENSEI_H

extern "C"
{
#include "hardware/gpio.h"
#include "SPI.h"
#include "I2C.h"
#include "graphics.h"
#include "input.h"
#include "audio.h"
}

class MeloperoSensei
{
public:
    MeloperoSensei();

    ~MeloperoSensei();

    /**** graphics interface ****/

    void clearScreen(uint8_t red, uint8_t green, uint8_t blue);

    void presentScreen();

    void drawPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);
    
    void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t red, uint8_t green, uint8_t blue);

    void drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue);

    void drawFillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue);

    void drawSprite(const uint16_t sprite[], int x, int y, uint16_t width, uint16_t height, uint16_t transparency = 0xE037 /*0x30FF00*/);

    void setTextFont(enum font_name name, enum font_size size);

    void setTextColor(uint8_t red, uint8_t green, uint8_t blue);

    void drawChar(uint16_t x, uint16_t y, char c, uint8_t red, uint8_t green, uint8_t blue);

    void print(uint8_t posX, uint8_t posY, const char *string, uint8_t red, uint8_t green, uint8_t blue);

    /**** input interface ****/

    ButtonState getButtonState(uint8_t button);

    /**** audio interface ****/

    void playNote(float frequency, uint32_t duration, float volume, bool sweep_direction, float sweep_time);

    /**** game loop ****/

    virtual void setup() {}

    virtual void update() {}

    virtual void draw() {}

    void run();

private:

    static const uint8_t SPI1_MOSI{11};
    static const uint8_t SPI1_MISO{8};
    static const uint8_t SPI1_SCK{10};
    static const uint8_t SPI1_CS{9};
    static const uint32_t SPI1_FREQUENCY{125 * 1000 * 1000};
    static const uint8_t SPI1_NUM_BITS{8};

    void SPI1Init()
    {
        spi_config config = { spi1, SPI1_MOSI, SPI1_MISO, SPI1_SCK, SPI1_CS, SPI1_FREQUENCY, SPI1_NUM_BITS, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST };
        SPI_init(&config);
    }

    static const uint8_t I2C1_SDA{6};
    static const uint8_t I2C1_SCL{7};
    static const uint32_t I2C1_FREQUENCY{100000};
    
    void I2C1Init()
    {
        i2c_config config = { i2c1, I2C1_SDA, I2C1_SCL, I2C1_FREQUENCY };
        I2C_init(&config);
    }

    static const uint8_t VSEN_PIN{17};

    void VSENEnable(bool enable)
    {
        gpio_init(VSEN_PIN);
        gpio_set_dir(VSEN_PIN, true);
        gpio_put(VSEN_PIN, enable);
    }

    void render()
    {
        graphics_clear_framebuffer(0x00, 0x00, 0x00);

        draw();

        graphics_present_framebuffer();
    }
};

#endif  // MELOPEROSENSEI_H