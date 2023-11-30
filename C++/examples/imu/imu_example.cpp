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

    // Loop forever
    while (true)
    {
        sensei.clearScreen(0, 0, 0);

        float* acceleration_mg = sensei.imuGetAccelerationMg();
        sprintf(msg, "x: %.2f  y: %.2f  z: %.2f", acceleration_mg[0], acceleration_mg[1], acceleration_mg[2]);
        sensei.print(0, 20, msg);

        float* angular_rate_mdps = sensei.imuGetAngularRateMdps();
        sprintf(msg, "x: %.2f  y: %.2f  z: %.2f", angular_rate_mdps[0], angular_rate_mdps[1], angular_rate_mdps[2]);
        sensei.print(0, 100, msg);

        sensei.presentScreen();
    }
}
