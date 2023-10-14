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

    sensei.clearScreen(0, 0, 0);
    sensei.print(0, 0, "IMU Initalized", 255, 255, 255);
    sensei.presentScreen();

    // Loop forever
    while (true)
    {

        imu.updateMeasurements();
        sensei.clearScreen(0, 0, 0);

        char acc_msg[128];
        char gyro_msg[128];
        char temp_msg[128];
        sprintf(acc_msg, "x: %f  y: %f  z: %f", imu.acceleration_mg[0], imu.acceleration_mg[1], imu.acceleration_mg[2]);
        sprintf(gyro_msg, "x: %f  y: %f  z: %f", imu.angular_rate_mdps[0], imu.angular_rate_mdps[1], imu.angular_rate_mdps[2]);
        sprintf(temp_msg, "t: %f", imu.temperature_degC);

        sensei.print(0, 0, acc_msg, 255, 255, 255);
        sensei.print(0, 20, gyro_msg, 255, 255, 255);
        sensei.print(0, 40, temp_msg, 255, 255, 255);

        sensei.presentScreen();
    }
}
