#ifndef Melopero_LSM6DSL_H_INCLUDED
#define Melopero_LSM6DSL_H_INCLUDED

extern "C"
{
#include "lsm6dsl-pid/lsm6dsl_reg.h"
}

class LSM6DSL
{
private:
    stmdev_ctx_t dev_ctx;

public:
    int16_t data_raw_acceleration[3];
    int16_t data_raw_angular_rate[3];
    int16_t data_raw_temperature;
    float acceleration_mg[3];
    float angular_rate_mdps[3];
    float temperature_degC;

    LSM6DSL();

    void testSetup(); // TODO: remove

    // void updateGyroscopeMeasurements();
    void updateAccelerationMeasurements();
    // void updateTemperatureMeasurements();
};

#endif // Melopero_LSM6DSL_H_INCLUDED