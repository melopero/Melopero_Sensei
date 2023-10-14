#include <stdio.h>
#include "pico/stdlib.h"
#include "../../src/MeloperoSensei.hpp"

extern "C"
{
#include "../../src/MPR121.h"
}

int main()
{

    MeloperoSensei sensei;

    gpio_init(5);
    gpio_set_dir(5, GPIO_IN);
    gpio_set_pulls(5, true, false);

    sensei.setTextFont(FREE_SANS_BOLD, SMALL);
    sensei.setTextColor(0x93, 0x70, 0xDB);

    sensei.print(20, 100, "Initializing Touch...");
    sensei.presentScreen();

    MPR121_reset();
    MPR121_stop();
    MPR121_set_thresholds(MPR121_TOUCH_THRESHOLD_DEFAULT, MPR121_RELEASE_THRESHOLD_DEFAULT);
    MPR121_run();

    sensei.clearScreen(0, 0, 0);
    sensei.print(15, 100, "Touch Initialized");
    sensei.presentScreen();

    uint16_t touch_status = 0;

    while (true)
    {

        bool value = gpio_get(5);
        sensei.clearScreen(0, 0, 0);

        // Update touch status. The touch status tells us which electrodes are being touched.
        // The value should be different from 0 if at least one electrode is being touched.
        MPR121_get_touch_status(&touch_status);

        char buf[32];
        sprintf(buf, "%x", touch_status);

        sensei.print(15, 100, buf);
        sensei.presentScreen();

        sensei.clearScreen(0, 0, 0);
    }
}
