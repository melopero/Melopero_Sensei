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
    imu.enableFreeFallDetection(true);
    imu.enableFreeFallInterrupt(false);

    sensei.clearScreen(0, 0, 0);
    sensei.print(0, 0, "IMU Initalized");
    sensei.presentScreen();

    while (true)
    {

        imu.updateInterruptSources();
        sensei.clearScreen(0, 0, 0);

        sprintf(msg, "Free fall: %d", imu.freeFallDetected);
        sensei.print(0, 20, msg);

        sensei.presentScreen();

        if (imu.freeFallDetected || imu.freeFallDetected)
            sleep_ms(2000);
    }
}
