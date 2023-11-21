#include "../../src/MeloperoSensei.hpp"
#include "heart.h"
#include "stdio.h"
#include "pico/time.h"

int main()
{
    MeloperoSensei sensei;

    sensei.enableButtonInterrupt(true);

    sensei.setTextFont(FREE_SANS_BOLD, VERY_SMALL);
    sensei.setTextColor(0xFF, 0xFF, 0xFF);

    sensei.clearScreen(0x00, 0xAA, 0xFF);
    sensei.print(10, 70, "light sensor calibration");
    sensei.print(10, 100, "cover the light sensor");
    sensei.presentScreen();

    sleep_ms(1000);

    sensei.clearScreen(0x00, 0xAA, 0xFF);
    sensei.print(10, 70, "light sensor calibration");
    sensei.print(10, 100, "cover the light sensor.");
    sensei.presentScreen();

    sleep_ms(1000);

    sensei.clearScreen(0x00, 0xAA, 0xFF);
    sensei.print(10, 70, "light sensor calibration");
    sensei.print(10, 100, "cover the light sensor..");
    sensei.presentScreen();

    sleep_ms(1000);

    sensei.clearScreen(0x00, 0xAA, 0xFF);
    sensei.print(10, 70, "light sensor calibration");
    sensei.print(10, 100, "cover the light sensor...");
    sensei.presentScreen();

    sensei.setLightMin();

    sleep_ms(1000);

    sensei.clearScreen(0x00, 0xAA, 0xFF);
    sensei.print(10, 70, "light sensor calibration");
    sensei.print(10, 100, "now uncover\n the light sensor");
    sensei.presentScreen();

    sleep_ms(1000);

    sensei.clearScreen(0x00, 0xAA, 0xFF);
    sensei.print(10, 70, "light sensor calibration");
    sensei.print(10, 100, "now uncover\n the light sensor.");
    sensei.presentScreen();

    sleep_ms(1000);

    sensei.clearScreen(0x00, 0xAA, 0xFF);
    sensei.print(10, 70, "light sensor calibration");
    sensei.print(10, 100, "now uncover\n the light sensor..");
    sensei.presentScreen();

    sleep_ms(1000);

    sensei.clearScreen(0x00, 0xAA, 0xFF);
    sensei.print(10, 70, "light sensor calibration");
    sensei.print(10, 100, "now uncover\n the light sensor...");
    sensei.presentScreen();

    sensei.setLightMax();

    sensei.clearScreen(0x00, 0xAA, 0xFF);
    sensei.print(10, 70, "calibration complete!");
    sensei.presentScreen();

    sleep_ms(2000);

    while (true)
    {
        sensei.clearScreen(0x00, 0xAA, 0xFF);

        sensei.drawSprite(heart, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

        uint16_t lightLevel = sensei.getLightLevel();
        uint8_t batteryLevel = sensei.getBatteryLevel();
        float temp = sensei.getTemperature();
        
        sensei.setTextFont(FREE_SANS_BOLD, REGULAR);
        sensei.setTextColor(0xFF, 0xFF, 0xFF);
        sensei.print(100, 100, std::to_string(lightLevel));
        sensei.print(100, 130, std::to_string(batteryLevel));

        char buf[5]{};
        sprintf(buf, "%.1f", temp);
        sensei.print(100, 160, buf);

        sensei.presentScreen();
        sleep_ms(500);
    }
}