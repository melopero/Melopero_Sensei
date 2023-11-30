#include <stdio.h>
#include "pico/stdlib.h"
#include "../../src/MeloperoSensei.hpp"

int main()
{

    MeloperoSensei sensei;
    char msg[128];

    sensei.setTextFont(FREE_MONO, SMALL);
    sensei.print(0, 0, "Initializing IMU...");
    sensei.presentScreen();

    sensei.imuInit();

    sensei.clearScreen(0, 0, 0);
    sensei.print(0, 0, "IMU Initalized");
    sensei.presentScreen();

    while (true)
    {
        sensei.clearScreen(0, 0, 0);

        bool singleTapDetected = sensei.imuGetSingleTapDetected();
        sprintf(msg, "Single Tap: %d", singleTapDetected);
        sensei.print(0, 20, msg);

        bool doubleTapDetected = sensei.imuGetDoubleTapDetected();
        sprintf(msg, "Double Tap: %d", doubleTapDetected);
        sensei.print(0, 40, msg);

        sensei.presentScreen();

        if (singleTapDetected || doubleTapDetected)
            sleep_ms(1000);
    }
}
