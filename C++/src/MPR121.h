#ifndef Melopero_MPR121_H_INCLUDED
#define Melopero_MPR121_H_INCLUDED

#include <stdint.h>

// I2C address
#define MPR121_I2C_DEFAULT_ADDRESS 0x5A

// Registers
#define MPR121_TOUCH_STATUS_0_REGISTER 0x00
#define MPR121_TOUCH_STATUS_1_REGISTER 0x01
// (0x2B~0x40) Baseline Filtering Control
#define MPR121_MAX_HALF_DELTA_RISING_REGISTER 0x2B      // Max half delta (rising)
#define MPR121_NOISE_HALF_DELTA_RISING_REGISTER 0x2C    // Noise half delta (rising)
#define MPR121_NOISE_COUNT_LIMIT_RISING_REGISTER 0x2D   // Noise count limit (rising)
#define MPR121_FILTER_DELAY_COUNT_RISING_REGISTER 0x2E  // Filter delay count (rising)
#define MPR121_MAX_HALF_DELTA_FALLING_REGISTER 0x2F     // Max half delta (falling)
#define MPR121_NOISE_HALF_DELTA_FALLING_REGISTER 0x30   // Noise half delta (falling)
#define MPR121_NOISE_COUNT_LIMIT_FALLING_REGISTER 0x31  // Noise count limit (falling)
#define MPR121_FILTER_DELAY_COUNT_FALLING_REGISTER 0x32 // Filter delay count (falling)
// There is no max half delta for touched
#define MPR121_NOISE_HALF_DELTA_TOUCHED_REGISTER 0x33   // Noise half delta (touched)
#define MPR121_NOISE_COUNT_LIMIT_TOUCHED_REGISTER 0x34  // Noise count limit (touched)
#define MPR121_FILTER_DELAY_COUNT_TOUCHED_REGISTER 0x35 // Filter delay count (touched)
#define MPR121_TOUCH_THRESHOLD_0_REGISTER 0x41
#define MPR121_RELEASE_THRESHOLD_0_REGISTER 0x42
#define MPR121_DEBOUNCE_REGISTER 0x5B
// (0x5C~0x5D) Filter and global CDC CDT configuration
#define MPR121_CONFIG1_REGISTER 0x5C // FFI (first filter iterations), CDC (charge/discharge current)
#define MPR121_CONFIG2_REGISTER 0x5D // CDT (charge/discharge time), SFI (second filter iterations), ESI (electrode sample interval)
#define MPR121_ELECTRODE_CONFIG_REGISTER 0x5E
#define MPR121_AUTO_CONFIG_0_REGISTER 0x7B
#define MPR121_AUTO_CONFIG_1_REGISTER 0x7C
#define MPR121_SOFT_RESET_REGISTER 0x80

// Default values
#define MPR121_TOUCH_THRESHOLD_DEFAULT 12
#define MPR121_RELEASE_THRESHOLD_DEFAULT 6

void MPR121_read_reg(uint8_t reg, uint8_t *val);
void MPR121_write_reg(uint8_t reg, uint8_t val);

void MPR121_auto_config();
void MPR121_get_touch_status(uint16_t *val);
void MPR121_reset();
void MPR121_run();
void MPR121_set_thresholds(uint8_t touch, uint8_t release);
void MPR121_stop();

#endif // Melopero_MPR121_H_INCLUDED