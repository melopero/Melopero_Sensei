#ifndef Melopero_LSM6DSL_H_INCLUDED
#define Melopero_LSM6DSL_H_INCLUDED

#include "lsm6dsl-pid/lsm6dsl_reg.h"

class LSM6DSL
{
private:
    stmdev_ctx_t dev_ctx;
    uint8_t i2c_address;

    /** Please note that is MANDATORY: return 0 -> no Error.**/
    int32_t platform_write(void *handle, uint8_t Reg, const uint8_t *Bufp, uint16_t len);
    int32_t platform_read(void *handle, uint8_t Reg, uint8_t *Bufp, uint16_t len);

public:
    int16_t data_raw_acceleration[3];
    int16_t data_raw_angular_rate[3];
    int16_t data_raw_temperature;
    float acceleration_mg[3];
    float angular_rate_mdps[3];
    float temperature_degC;

    void initI2C(uint8_t address);

    void testSetup(); // TODO: remove

    // void updateGyroscopeMeasurements();
    void updateAccelerationMeasurements();
    // void updateTemperatureMeasurements();
};

#endif // Melopero_LSM6DSL_H_INCLUDED