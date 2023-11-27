#ifndef Melopero_LSM6DSL_H_INCLUDED
#define Melopero_LSM6DSL_H_INCLUDED

extern "C"
{
#include "lsm6dsl-pid/lsm6dsl_reg.h"
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

enum class AccelerometerScale
{
    XL_2g = 0,
    XL_16g = 1,
    XL_4g = 2,
    XL_8g = 3,
    XL_ND = 4
};

enum class GyroscopeOutputDataRate
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
    ODR_ND = 11
};

enum class GyroscopeScale
{
    GY_250dps = 0,
    GY_125dps = 1,
    GY_500dps = 2,
    GY_1000dps = 4,
    GY_2000dps = 6,
    GY_ND = 7
};

enum class LSM6DSLInterruptPin
{
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
    bool freeFallDetected;
    bool singleTapDetected;
    bool doubleTapDetected;
    uint16_t steps;

    LSM6DSL();

    void reset();
    void setOutputDataRates(AccelerometerOutputDataRate acc_odr, GyroscopeOutputDataRate gyro_odr);
    void setScales(AccelerometerScale acc_scale, GyroscopeScale gyro_scale);
    // TODO: filters
    // TODO: threshold settings for interrupts
    void enableTapDetection(bool enable);
    void enableFreeFallInterrupt(bool enable, LSM6DSLInterruptPin pin = LSM6DSLInterruptPin::PIN_1);
    void enableSingleTapInterrupt(bool enable, LSM6DSLInterruptPin pin = LSM6DSLInterruptPin::PIN_1);
    void enableDoubleTapInterrupt(bool enable, LSM6DSLInterruptPin pin = LSM6DSLInterruptPin::PIN_1);

    void enablePedometer(bool enable);
    void resetStepCounter();
    void updateStepCounter();

    void updateInterruptSources();
    void updateMeasurements();
};

#endif // Melopero_LSM6DSL_H_INCLUDED