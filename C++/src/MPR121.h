#ifndef Melopero_MPR121_H_INCLUDED
#define Melopero_MPR121_H_INCLUDED

#include <stdint.h>

// I2C address
#define MPR121_I2C_DEFAULT_ADDRESS 0x5A

// Registers
#define MPR121_TOUCH_STATUS_0_REGISTER 0x00
#define MPR121_TOUCH_STATUS_1_REGISTER 0x01
#define MPR121_ELECTRODE_CONFIG_REGISTER 0x5E
#define MPR121_AUTO_CONFIG_0_REGISTER 0x7B
#define MPR121_AUTO_CONFIG_1_REGISTER 0x7C
#define MPR121_SOFT_RESET_REGISTER 0x80

void MPR121_read_reg(uint8_t reg, uint8_t *val);
void MPR121_write_reg(uint8_t reg, uint8_t val);

void MPR121_auto_config();
void MPR121_get_touch_status(uint16_t *val);
void MPR121_reset();
void MPR121_run();
void MPR121_stop();

#endif // Melopero_MPR121_H_INCLUDED