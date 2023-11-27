#ifndef MELOPEROSENSEI_H
#define MELOPEROSENSEI_H

#include <string>

extern "C"
{
#include "hardware/gpio.h"
#include "SPI.h"
#include "I2C.h"
#include "graphics.h"
#include "input.h"
#include "audio.h"
#include "analog.h"
#include "MPR121.h"
#include "LSM6DSL.hpp"
}

class MeloperoSensei
{
public:
    MeloperoSensei();

    MeloperoSensei(void *buffer);

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

    void drawChar(uint16_t x, uint16_t y, char c);

    void print(uint8_t posX, uint8_t posY, std::string const &string);

    /**** input interface ****/

    ButtonState getButtonState(uint8_t button);

    bool isButtonPressed(uint8_t button);

    void enableButtonInterrupt(bool enable);

    bool getButtonIRQState(uint8_t button);

    uint8_t getInputInterruptRegister();

    /**** audio interface ****/

    void playNote(float frequency, uint32_t duration, float volume, bool sweep_direction, float sweep_time);

    /**** analog interface ****/

    void setLightMin();

    void setLightMax();

    uint8_t getLightLevel();

    uint8_t getBatteryLevel();

    float getTemperature();

    /**** touch sensor interface ****/

    void touch_init();

    uint16_t get_touch();

    /**** imu sensor interface ****/
     

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
    void SPI1Init();

    static const uint8_t I2C1_SDA{6};
    static const uint8_t I2C1_SCL{7};
    static const uint32_t I2C1_FREQUENCY{100000};
    void I2C1Init();

    static const uint8_t VSEN_PIN{17};
    void VSENEnable(bool enable);

    void render();

    bool graphicsMP = false;
};

#endif // MELOPEROSENSEI_H