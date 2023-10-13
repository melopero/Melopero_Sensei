#include <stdio.h>
#include "pico/stdlib.h"
#include "../../src/MeloperoSensei.hpp"
#include "../../src/LSM6DSL.hpp"

int main()
{

    MeloperoSensei sensei;
    LSM6DSL imu;
    sensei.print(0, 0, "Initializing IMU...", 255, 255, 255);
    sensei.presentScreen();

    imu.testSetup();

    sensei.clearScreen(0,0,0);
    sensei.print(0, 0, "IMU Initalized", 255, 255, 255);
    sensei.presentScreen();

    int counter = 0;

    // Loop forever
    while (true)
    {

        // Blink LED
        imu.updateAccelerationMeasurements();
        sensei.clearScreen(0, 0, 0);
        //counter++;
        char msg[128];
        sprintf(msg, "x: %f  y: %f  z: %f", imu.acceleration_mg[0], imu.acceleration_mg[1], imu.acceleration_mg[2]);
        sensei.print(0, 0, msg, 255, 255, 255);
        //sensei.print(counter, counter, "test ", 255, 255, 255);
        sensei.presentScreen();
    }
}
