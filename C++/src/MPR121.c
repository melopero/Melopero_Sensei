#include "MPR121.h"
#include "I2C.h"

void MPR121_auto_config()
{
    MPR121_write_reg(MPR121_AUTO_CONFIG_0_REGISTER, 0xFF);
    MPR121_write_reg(MPR121_AUTO_CONFIG_1_REGISTER, 0);
}

void MPR121_get_touch_status(uint16_t *val)
{
    uint8_t status_0;
    uint8_t status_1;
    MPR121_read_reg(MPR121_TOUCH_STATUS_0_REGISTER, &status_0);
    MPR121_read_reg(MPR121_TOUCH_STATUS_1_REGISTER, &status_1);
    *val = (status_0 | (status_1 << 8)) & 0x0EFF;
}

void MPR121_reset()
{
    // Soft reset
    MPR121_write_reg(MPR121_SOFT_RESET_REGISTER, 0x63);

    // Reset electrode configuration to defaults - enter stop mode
    // Config registers are read-only unless in stop mode
    MPR121_stop();


    // Check CDT, SFI, ESI configuration is at defaults
    // A soft reset puts CONFIG2 (0x5D) at 0x24
    // Charge Discharge Time, CDT=1 (0.5us charge time)
    // Second Filter Iterations, SFI=0 (4x samples taken)
    // Electrode Sample Interval, ESI=4 (16ms period)
    // TODO: check content of CONFIG2 register and see if soft reset worked

    // Set touch and release trip thresholds
    MPR121_set_thresholds(MPR121_TOUCH_THRESHOLD_DEFAULT, MPR121_RELEASE_THRESHOLD_DEFAULT);

    // Configure electrode filtered data and baseline registers
    MPR121_write_reg(MPR121_MAX_HALF_DELTA_RISING_REGISTER, 0x01);
    MPR121_write_reg(MPR121_MAX_HALF_DELTA_FALLING_REGISTER, 0x01);
    MPR121_write_reg(MPR121_NOISE_HALF_DELTA_RISING_REGISTER, 0x01);
    MPR121_write_reg(MPR121_NOISE_HALF_DELTA_FALLING_REGISTER, 0x05);
    MPR121_write_reg(MPR121_NOISE_HALF_DELTA_TOUCHED_REGISTER, 0x00);
    MPR121_write_reg(MPR121_NOISE_COUNT_LIMIT_RISING_REGISTER, 0x0E);
    MPR121_write_reg(MPR121_NOISE_COUNT_LIMIT_FALLING_REGISTER, 0x01);
    MPR121_write_reg(MPR121_NOISE_COUNT_LIMIT_TOUCHED_REGISTER, 0x00);
    MPR121_write_reg(MPR121_FILTER_DELAY_COUNT_RISING_REGISTER, 0x00);
    MPR121_write_reg(MPR121_FILTER_DELAY_COUNT_FALLING_REGISTER, 0x00);
    MPR121_write_reg(MPR121_FILTER_DELAY_COUNT_TOUCHED_REGISTER, 0x00);

    // Set config registers
    // Debounce Touch, DT=0 (increase up to 7 to reduce noise)
    // Debounce Release, DR=0 (increase up to 7 to reduce noise)
    MPR121_write_reg(MPR121_DEBOUNCE_REGISTER, 0x00);
    // First Filter Iterations, FFI=0 (6x samples taken)
    // Charge Discharge Current, CDC=16 (16uA)
    MPR121_write_reg(MPR121_CONFIG1_REGISTER, 0x10);
    // Charge Discharge Time, CDT=1 (0.5us charge time)
    // Second Filter Iterations, SFI=0 (4x samples taken)
    // Electrode Sample Interval, ESI=0 (1ms period)
    MPR121_write_reg(MPR121_CONFIG2_REGISTER, 0x20);

    // Enable all electrodes - enter run mode
    // Calibration Lock, CL=10 (baseline tracking enabled, initial value 5 high bits)
    // Proximity Enable, ELEPROX_EN=0 (proximity detection disabled)
    // Electrode Enable, ELE_EN=15 (enter run mode for 12 electrodes)
    MPR121_run();
}

void MPR121_run()
{
    // Run mode: enable all electrodes
    MPR121_write_reg(MPR121_ELECTRODE_CONFIG_REGISTER, 0x8F);
}

void MPR121_stop()
{
    // Stop mode: disable electrodes
    // TODO: keep first two bits of register
    MPR121_write_reg(MPR121_ELECTRODE_CONFIG_REGISTER, 0x00);
}

void MPR121_read_reg(uint8_t reg, uint8_t *val)
{
    I2C_read_reg(MPR121_I2C_DEFAULT_ADDRESS, reg, val, 1, true);
}

void MPR121_write_reg(uint8_t reg, uint8_t val)
{   
    const uint8_t write[] = { reg, val };
    I2C_write(MPR121_I2C_DEFAULT_ADDRESS, write, 2, true);
    //I2C_write_reg(MPR121_I2C_DEFAULT_ADDRESS, reg, &val, 1, true);
}

void MPR121_set_thresholds(uint8_t touch, uint8_t release)
{
    for (uint8_t i = 0; i < 12; i++)
    {
        MPR121_write_reg(MPR121_TOUCH_THRESHOLD_0_REGISTER + 2 * i, touch);
        MPR121_write_reg(MPR121_RELEASE_THRESHOLD_0_REGISTER + 2 * i, release);
    }
}
