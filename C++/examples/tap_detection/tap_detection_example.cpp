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
    imu.setOutputDataRates(AccelerometerOutputDataRate::ODR_416Hz, GyroscopeOutputDataRate::ODR_416Hz);
    imu.setScales(AccelerometerScale::XL_2g, GyroscopeScale::GY_2000dps);
    imu.enableTapDetection(true);
    imu.enableSingleTapInterrupt(false);
    imu.enableDoubleTapInterrupt(false);

    //imu.test();
    sensei.clearScreen(0, 0, 0);
    sensei.print(0, 0, "IMU Initalized");
    sensei.presentScreen();

    while (true)
    {

        imu.updateInterruptSources();
        sensei.clearScreen(0, 0, 0);

        sprintf(msg, "Single Tap: %d", imu.singleTapDetected);
        sensei.print(0, 20, msg);
        sprintf(msg, "Double Tap: %d", imu.doubleTapDetected);
        sensei.print(0, 40, msg);

        sensei.presentScreen();

        if (imu.singleTapDetected || imu.doubleTapDetected) sleep_ms(1000);
    }
}
