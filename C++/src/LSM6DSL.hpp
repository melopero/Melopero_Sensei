#ifndef Melopero_LSM6DSL_H_INCLUDED
#define Melopero_LSM6DSL_H_INCLUDED


extern "C"
{
#include "lsm6dsl-pid/lsm6dsl_reg.h"
}

// TODO: use LSM6DSL status instead of returning void
enum class LSM6DSLStatus {
    NO_ERROR = 0,
    I2C_COMMUNICATION_ERROR = -1
}

enum class AccelerometerOutputDataRate
{
    ODR_OFF = 0,
    ODR_12Hz5 = 1,
    ODR_26Hz = 2,
    ODR_52Hz = 3,
    ODR_104Hz = 4,
    ODR_208Hz = 5,
    ODR_416Hz = 6,
    ODR_833Hz = 7,
    ODR_1k66Hz = 8,
    ODR_3k33Hz = 9,
    ODR_6k66Hz = 10,
    ODR_1Hz6 = 11,
    ODR_ND = 12
};

enum class LSM6DSLInterruptPin {
    PIN_1 = 0,
    PIN_2 = 1
};

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

    void reset();
    // TODO: finish
    void setOutputDataRates(AccelerometerOutputDataRate acc_odr);
    // TODO: void setScales();
    // TODO: filters
    // TODO: threshold settings for interrupts
    void setFreeFallInterrupt(bool enable, LSM6DSLInterruptPin pin = LSM6DSLInterruptPin::PIN_1);
    void setSingleTapInterrupt(bool enable, LSM6DSLInterruptPin pin = LSM6DSLInterruptPin::PIN_1);
    void setDoubleTapInterrupt(bool enable, LSM6DSLInterruptPin pin = LSM6DSLInterruptPin::PIN_1);

    void testSetup(); // TODO: remove

    LSM6DSLStatus updateMeasurements();
};

#endif // Melopero_LSM6DSL_H_INCLUDED