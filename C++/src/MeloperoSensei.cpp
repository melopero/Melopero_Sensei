#include "MeloperoSensei.hpp"

MeloperoSensei::MeloperoSensei()
{
    SPI1Init();
    I2C1Init();
    graphics_init();
    input_init();
    VSENEnable(true);
    audio_init();
    analog_init();
}

MeloperoSensei::MeloperoSensei(void *buffer)
{
    SPI1Init();
    I2C1Init();
    graphics_init_mp(buffer);
    graphicsMP = true;
    input_init();
    VSENEnable(true);
    audio_init();
    analog_init();
    
}

MeloperoSensei::~MeloperoSensei()
{
    if (!graphicsMP)
        graphics_deinit();
}

/**** graphics interface ****/

void MeloperoSensei::clearScreen(uint8_t red, uint8_t green, uint8_t blue)
{
    graphics_clear_framebuffer(red, green, blue);
}

void MeloperoSensei::presentScreen()
{
    graphics_present_framebuffer();
}

void MeloperoSensei::drawPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue)
{
    graphics_draw_pixel(x, y, red, green, blue);
}

void MeloperoSensei::drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t red, uint8_t green, uint8_t blue)
{
    graphics_draw_line(x1, y1, x2, y2, red, green, blue);
}

void MeloperoSensei::drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue)
{
    graphics_draw_rect(x, y, width, height, red, green, blue);
}

void MeloperoSensei::drawFillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue)
{
    graphics_draw_fill_rect(x, y, width, height, red, green, blue);
}

void MeloperoSensei::drawSprite(const uint16_t sprite[], int x, int y, uint16_t width, uint16_t height, uint16_t transparency )
{
    graphics_draw_sprite(sprite, x, y, width, height, transparency);
}

void MeloperoSensei::setTextFont(enum font_name name, enum font_size size)
{
    graphics_set_text_font(name, size);
}

void MeloperoSensei::setTextColor(uint8_t red, uint8_t green, uint8_t blue)
{
    graphics_set_text_color(red, green, blue);
}

void MeloperoSensei::drawChar(uint16_t x, uint16_t y, char c)
{
    graphics_draw_char(x, y, c);
}

void MeloperoSensei::print(uint8_t posX, uint8_t posY, const std::string &string)
{
    graphics_set_cursor_x(posX);
    graphics_set_cursor_y(posY);
    graphics_print(string.c_str());
}

/**** input interface ****/

ButtonState MeloperoSensei::getButtonState(uint8_t button)
{
    return input_get_button_state(button);
}

bool MeloperoSensei::isButtonPressed(uint8_t button)
{
    //return input_is_pressed(button);
    return input_get_button(button);
}

void MeloperoSensei::enableButtonInterrupt(bool enable)
{
    input_enable_interrupt(enable);
}

bool MeloperoSensei::getButtonIRQState(uint8_t button)
{
    return input_get_button_irq_state(button);
}

uint8_t MeloperoSensei::getInputInterruptRegister()
{
    return input_get_interrupt_register();
}

/**** audio interface ****/

void MeloperoSensei::playNote(float frequency, uint32_t duration, float volume, bool sweep_direction, float sweep_time)
{
    audio_play_note(frequency, duration, volume, sweep_direction, sweep_time);
}

/**** analog interface ****/

void MeloperoSensei::setLightMin()
{
    analog_set_light_min();
}

void MeloperoSensei::setLightMax()
{
    analog_set_light_max();
}

uint8_t MeloperoSensei::getLightLevel()
{
    return analog_read_light();
}
    
uint8_t MeloperoSensei::getBatteryLevel()
{
    return analog_read_battery();
}

float MeloperoSensei::getTemperature()
{
    return analog_read_temperature();
}


/**** touch sensor ****/

void MeloperoSensei::touch_init()
{
    MPR121_reset();
}

uint16_t MeloperoSensei::get_touch()
{
    uint16_t result;
    MPR121_get_touch_status(&result);
    return result;
}

/**** imu sensor ****/

void MeloperoSensei::imuInit()
{
    imuReset();
    imuSetOutputDataRates(AccelerometerOutputDataRate::ODR_416Hz, GyroscopeOutputDataRate::ODR_416Hz);
    imuSetScales(AccelerometerScale::XL_2g, GyroscopeScale::GY_2000dps);
    imuEnablePedometer(true);
    imuResetStepCounter();
    imuEnableTapDetection(true);
    imuEnableFreeFallDetection(true);
    imuEnableInterrupts(false, false, false);
}

void MeloperoSensei::imuReset()
{
    imu.reset();
}

void MeloperoSensei::imuSetOutputDataRates(AccelerometerOutputDataRate acc_odr, GyroscopeOutputDataRate gyro_odr)
{
    imu.setOutputDataRates(acc_odr, gyro_odr);
}

void MeloperoSensei::imuSetScales(AccelerometerScale acc_scale, GyroscopeScale gyro_scale)
{
    imu.setScales(acc_scale, gyro_scale);
}

void MeloperoSensei::imuEnableTapDetection(bool enable)
{
    imu.enableTapDetection(enable);
}

void MeloperoSensei::imuEnableFreeFallDetection(bool enable)
{
    imu.enableFreeFallDetection(enable);
}

void MeloperoSensei::imuEnableInterrupts(bool enable_single_tap_interrupt, bool enable_double_tap_interrupt, bool enable_free_fall_interrupt)
{
    imu.enableSingleTapInterrupt(enable_single_tap_interrupt);
    imu.enableDoubleTapInterrupt(enable_double_tap_interrupt);
    imu.enableFreeFallInterrupt(enable_free_fall_interrupt);
}

void MeloperoSensei::imuEnablePedometer(bool enable)
{
    imu.enablePedometer(enable);
}

void MeloperoSensei::imuResetStepCounter()
{
    imu.resetStepCounter();
}

uint16_t MeloperoSensei::imuGetSteps()
{
    imu.updateStepCounter();
    return imu.steps;
}

bool MeloperoSensei::imuGetFreeFallDetected()
{
    imu.updateInterruptSources();
    return imu.freeFallDetected;
}

bool MeloperoSensei::imuGetSingleTapDetected()
{
    imu.updateInterruptSources();
    return imu.singleTapDetected;
}

bool MeloperoSensei::imuGetDoubleTapDetected()
{
    imu.updateInterruptSources();
    return imu.doubleTapDetected;
}

float* MeloperoSensei::imuGetAccelerationMg()
{
    imu.updateMeasurements();
    return imu.acceleration_mg;
}

float* MeloperoSensei::imuGetAngularRateMdps()
{
    imu.updateMeasurements();
    return imu.angular_rate_mdps;
}


/**** game loop ****/

void MeloperoSensei::run()
{
    setup();

    while (true)
    {
        update();
        render();
    }
}

/**** private implementation ****/

void MeloperoSensei::SPI1Init()
{
    spi_config config = { spi1, SPI1_MOSI, SPI1_MISO, SPI1_SCK, SPI1_CS, SPI1_FREQUENCY, SPI1_NUM_BITS, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST };
    SPI_init(&config);
}

void MeloperoSensei::I2C1Init()
{
    i2c_config config = { i2c1, I2C1_SDA, I2C1_SCL, I2C1_FREQUENCY };
    I2C_init(&config);
}

void MeloperoSensei::VSENEnable(bool enable)
{
    gpio_init(VSEN_PIN);
    gpio_set_dir(VSEN_PIN, true);
    gpio_put(VSEN_PIN, enable);
}

void MeloperoSensei::render()
{
    graphics_clear_framebuffer(0x00, 0x00, 0x00);

    draw();

    graphics_present_framebuffer();
}