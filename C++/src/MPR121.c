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
    // Soft reset: write 0x63 to register 0x80
    MPR121_write_reg(MPR121_SOFT_RESET_REGISTER, 0x63);
}

void MPR121_run()
{
    // Run mode: enable the first 8 electrodes
    MPR121_write_reg(MPR121_ELECTRODE_CONFIG_REGISTER, 0x08);
}

void MPR121_stop()
{
    // Stop mode: write 0x00 to register 0x5E
    // TODO: keep first two bits of register
    MPR121_write_reg(MPR121_ELECTRODE_CONFIG_REGISTER, 0x00);
}

void MPR121_read_reg(uint8_t reg, uint8_t *val)
{
    I2C_read_reg(MPR121_I2C_DEFAULT_ADDRESS, reg, val, 1, true);
}

void MPR121_write_reg(uint8_t reg, uint8_t val)
{
    I2C_write_reg(MPR121_I2C_DEFAULT_ADDRESS, reg, &val, 1, true);
}
