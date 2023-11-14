#include <stdio.h>
#include "pico/stdlib.h"
#include "../../src/MeloperoSensei.hpp"
#include "../../src/LSM6DSL.hpp"

int main()
{

    MeloperoSensei sensei;
    LSM6DSL imu;
    char acc_msg[128];
    char gyro_msg[128];
    char temp_msg[128];

    sensei.setTextFont(FREE_MONO, SMALL);
    sensei.print(0, 0, "Initializing IMU...");
    sensei.presentScreen();

    imu.testSetup();

    sensei.clearScreen(0, 0, 0);
    sensei.print(0, 0, "IMU Initalized");
    sensei.presentScreen();

    // Loop forever
    while (true)
    {

        imu.updateMeasurements();
        sensei.clearScreen(0, 0, 0);

        sprintf(acc_msg, "x: %.2f  y: %.2f  z: %.2f", imu.acceleration_mg[0], imu.acceleration_mg[1], imu.acceleration_mg[2]);
        sprintf(gyro_msg, "x: %.2f  y: %.2f  z: %.2f", imu.angular_rate_mdps[0], imu.angular_rate_mdps[1], imu.angular_rate_mdps[2]);
        sprintf(temp_msg, "t: %.2f", imu.temperature_degC);

        sensei.print(0, 20, acc_msg);
        sensei.print(0, 100, gyro_msg);
        sensei.print(0, 180, temp_msg);

        sensei.presentScreen();
    }
}
