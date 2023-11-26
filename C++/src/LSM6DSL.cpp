#include "LSM6DSL.hpp"

extern "C"
{
#include "I2C.h"
#include "string.h"
}

int32_t platform_read(void *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    I2C_read_reg(LSM6DSL_ID, reg, buf, len, true);
    return 0;
}

int32_t platform_write(void *handle, uint8_t reg, const uint8_t *buf, uint16_t len)
{
    I2C_write_reg(LSM6DSL_ID, reg, buf, len, true);
    return 0;
}

LSM6DSL::LSM6DSL()
{
    dev_ctx.read_reg = platform_read;
    dev_ctx.write_reg = platform_write;
}

void LSM6DSL::reset()
{
    // Restore default configuration
    uint8_t rst;
    lsm6dsl_reset_set(&dev_ctx, PROPERTY_ENABLE);

    do
    {
        lsm6dsl_reset_get(&dev_ctx, &rst);
    } while (rst);
}

void LSM6DSL::setOutputDataRates(AccelerometerOutputDataRate acc_odr, GyroscopeOutputDataRate gyro_odr)
{
    lsm6dsl_xl_data_rate_set(&dev_ctx, static_cast<lsm6dsl_odr_xl_t>(acc_odr));
    lsm6dsl_gy_data_rate_set(&dev_ctx, static_cast<lsm6dsl_odr_g_t>(gyro_odr));
}

void LSM6DSL::setScales(AccelerometerScale acc_scale, GyroscopeScale gyro_scale)
{
    lsm6dsl_xl_full_scale_set(&dev_ctx, static_cast<lsm6dsl_fs_xl_t>(acc_scale));
    lsm6dsl_gy_full_scale_set(&dev_ctx, static_cast<lsm6dsl_fs_g_t>(gyro_scale));
}

void LSM6DSL::testSetup()
{
    // Restore default configuration
    uint8_t rst;
    lsm6dsl_reset_set(&dev_ctx, PROPERTY_ENABLE);

    do
    {
        lsm6dsl_reset_get(&dev_ctx, &rst);
    } while (rst);

    /* Enable Block Data Update */
    lsm6dsl_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
    /* Set Output Data Rate */
    lsm6dsl_xl_data_rate_set(&dev_ctx, LSM6DSL_XL_ODR_12Hz5);
    lsm6dsl_gy_data_rate_set(&dev_ctx, LSM6DSL_GY_ODR_12Hz5);
    /* Set full scale */
    lsm6dsl_xl_full_scale_set(&dev_ctx, LSM6DSL_2g);
    lsm6dsl_gy_full_scale_set(&dev_ctx, LSM6DSL_2000dps);
    /* Configure filtering chain(No aux interface) */
    /* Accelerometer - analog filter */
    lsm6dsl_xl_filter_analog_set(&dev_ctx, LSM6DSL_XL_ANA_BW_400Hz);
    /* Accelerometer - LPF1 path ( LPF2 not used )*/
    lsm6dsl_xl_lp1_bandwidth_set(&dev_ctx, LSM6DSL_XL_LP1_ODR_DIV_4);
    /* Accelerometer - LPF1 + LPF2 path */
    lsm6dsl_xl_lp2_bandwidth_set(&dev_ctx,
                                 LSM6DSL_XL_LOW_NOISE_LP_ODR_DIV_100);
    /* Accelerometer - High Pass / Slope path */
    lsm6dsl_xl_reference_mode_set(&dev_ctx, PROPERTY_DISABLE);
    lsm6dsl_xl_hp_bandwidth_set(&dev_ctx, LSM6DSL_XL_HP_ODR_DIV_100);
    /* Gyroscope - filtering chain */
    lsm6dsl_gy_band_pass_set(&dev_ctx, LSM6DSL_HP_260mHz_LP1_STRONG);

    /* Enable Block Data Update */
    lsm6dsl_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
}

void LSM6DSL::enableFreeFallInterrupt(bool enable, LSM6DSLInterruptPin pin)
{
    if (pin == LSM6DSLInterruptPin::PIN_1)
    {
        lsm6dsl_int1_route_t int1_value;
        lsm6dsl_pin_int1_route_get(&dev_ctx, &int1_value);
        int1_value.int1_ff = enable ? 1 : 0;
        lsm6dsl_pin_int1_route_set(&dev_ctx, int1_value);
    }
    else
    {
        lsm6dsl_int2_route_t int2_value;
        lsm6dsl_pin_int2_route_get(&dev_ctx, &int2_value);
        int2_value.int2_ff = enable ? 1 : 0;
        lsm6dsl_pin_int2_route_set(&dev_ctx, int2_value);
    }
}

void LSM6DSL::enableSingleTapInterrupt(bool enable, LSM6DSLInterruptPin pin)
{
    if (pin == LSM6DSLInterruptPin::PIN_1)
    {
        lsm6dsl_int1_route_t int1_value;
        lsm6dsl_pin_int1_route_get(&dev_ctx, &int1_value);
        int1_value.int1_single_tap = enable ? 1 : 0;
        lsm6dsl_pin_int1_route_set(&dev_ctx, int1_value);
    }
    else
    {
        lsm6dsl_int2_route_t int2_value;
        lsm6dsl_pin_int2_route_get(&dev_ctx, &int2_value);
        int2_value.int2_single_tap = enable ? 1 : 0;
        lsm6dsl_pin_int2_route_set(&dev_ctx, int2_value);
    }
}

void LSM6DSL::enableDoubleTapInterrupt(bool enable, LSM6DSLInterruptPin pin)
{
    if (pin == LSM6DSLInterruptPin::PIN_1)
    {
        lsm6dsl_int1_route_t int1_value;
        lsm6dsl_pin_int1_route_get(&dev_ctx, &int1_value);
        int1_value.int1_double_tap = enable ? 1 : 0;
        lsm6dsl_pin_int1_route_set(&dev_ctx, int1_value);
    }
    else
    {
        lsm6dsl_int2_route_t int2_value;
        lsm6dsl_pin_int2_route_get(&dev_ctx, &int2_value);
        int2_value.int2_double_tap = enable ? 1 : 0;
        lsm6dsl_pin_int2_route_set(&dev_ctx, int2_value);
    }
}

void LSM6DSL::enablePedometer(bool enable)
{
    // Pedometer functions work at 26 Hz, so the accelerometer ODR must be set at a value of 26 Hz or higher.
    lsm6dsl_pedo_sens_set(&dev_ctx, enable? 1 : 0);
}

void LSM6DSL::resetStepCounter()
{
    lsm6dsl_pedo_step_reset_set(&dev_ctx, 1);
    do {
        updateStepCounter();
    }
    while (steps != 0);
    // After the counter resets, the PEDO_RST_STEP bit is not automatically set back to 0.
    // So we need to manually set it to 0.
    lsm6dsl_pedo_step_reset_set(&dev_ctx, 0);
}

void LSM6DSL::updateStepCounter()
{
    uint8_t steps_buffer[2] = {0,0};
    lsm6dsl_read_reg(&dev_ctx, LSM6DSL_STEP_COUNTER_L, steps_buffer, 2);
    steps = steps_buffer[0] | (steps_buffer[1] << 8);
}

void LSM6DSL::updateInterruptSources()
{
    lsm6dsl_all_sources_t int_src;
    lsm6dsl_all_sources_get(&dev_ctx, &int_src);
    singleTapDetected = int_src.tap_src.single_tap == 1;
    doubleTapDetected = int_src.tap_src.double_tap == 1;
    freeFallDetected = int_src.wake_up_src.ff_ia == 1;
}

void LSM6DSL::updateMeasurements()
{
    // Read output only if new value is available
    lsm6dsl_reg_t reg;
    lsm6dsl_status_reg_get(&dev_ctx, &reg.status_reg);

    // Check that new accelerometer data is available
    if (reg.status_reg.xlda)
    {
        memset(data_raw_acceleration, 0x00, 3 * sizeof(int16_t));
        lsm6dsl_acceleration_raw_get(&dev_ctx, data_raw_acceleration);
        acceleration_mg[0] = lsm6dsl_from_fs2g_to_mg(data_raw_acceleration[0]);
        acceleration_mg[1] = lsm6dsl_from_fs2g_to_mg(data_raw_acceleration[1]);
        acceleration_mg[2] = lsm6dsl_from_fs2g_to_mg(data_raw_acceleration[2]);
    }

    if (reg.status_reg.gda)
    {
        int16_t data_raw_angular_rate[3];
        memset(data_raw_angular_rate, 0x00, 3 * sizeof(int16_t));
        lsm6dsl_angular_rate_raw_get(&dev_ctx, data_raw_angular_rate);
        angular_rate_mdps[0] = lsm6dsl_from_fs2000dps_to_mdps(
            data_raw_angular_rate[0]);
        angular_rate_mdps[1] = lsm6dsl_from_fs2000dps_to_mdps(
            data_raw_angular_rate[1]);
        angular_rate_mdps[2] = lsm6dsl_from_fs2000dps_to_mdps(
            data_raw_angular_rate[2]);
    }

    if (reg.status_reg.tda)
    {
        /* Read temperature data */
        memset(&data_raw_temperature, 0x00, sizeof(int16_t));
        lsm6dsl_temperature_raw_get(&dev_ctx, &data_raw_temperature);
        temperature_degC = lsm6dsl_from_lsb_to_celsius(
            data_raw_temperature);
    }
}
