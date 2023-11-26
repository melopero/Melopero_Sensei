#include <stdio.h>
#include "pico/stdlib.h"
#include "../../src/MeloperoSensei.hpp"
#include "../../src/LSM6DSL.hpp"

int main()
{

    MeloperoSensei sensei;
    LSM6DSL imu;
    char msg[128];

    sensei.setTextFont(FREE_MONO, SMALL);
    sensei.print(0, 0, "Initializing IMU...");
    sensei.presentScreen();

    imu.reset();
    imu.setOutputDataRates(AccelerometerOutputDataRate::ODR_52Hz, GyroscopeOutputDataRate::ODR_52Hz);
    imu.setScales(AccelerometerScale::XL_2g, GyroscopeScale::GY_2000dps);
    imu.enablePedometer(true);
    imu.resetStepCounter();

    sensei.clearScreen(0, 0, 0);
    sensei.print(0, 0, "IMU Initalized");
    sensei.presentScreen();

    while (true)
    {

        imu.updateStepCounter();
        sensei.clearScreen(0, 0, 0);

        sprintf(msg, "steps: %d", imu.steps);

        sensei.print(0, 20, msg);

        sensei.presentScreen();
    }
}
