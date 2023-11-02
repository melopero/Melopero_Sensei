#include "io_expander.h"

#define ADDRESS                             0x43

/**** IO expander registers ****/
#define DEVICE_ID_AND_CONTROL               0x01
#define IO_DIRECTION                        0x03
#define OUTPUT_PORT_REGISTER                0x05
#define OUTPUT_HIGH_IMPEDANCE               0x07
#define INPUT_DEFAULT_STATE                 0x09
#define PULL_UP_PULL_DOWN_ENABLE            0x0B
#define PULL_UP_PULL_DOWN_SELECT            0x0D
#define INPUT_STATUS_REGISTER               0x0F
#define INTERRUPT_MASK_REGISTER             0x11
#define INTERRUPT_STATUS_REGISTER           0x13

static uint8_t io_expander_read_register(uint8_t reg);
static void io_expander_write_register(uint8_t reg, uint8_t value);
static void io_expander_set_register_bit(uint8_t reg, uint8_t pin, bool value);
static bool io_expander_get_register_bit(uint8_t reg, uint8_t pin);

void io_expander_init()
{
    io_expander_set_register_bit(DEVICE_ID_AND_CONTROL, 1 << 0, PIN_HIGH); 
    (void)io_expander_get_register_bit(DEVICE_ID_AND_CONTROL, 1 << 1);
}

uint8_t io_expander_read_register(uint8_t reg)
{
    I2C_write(ADDRESS, &reg, 1, false);

    uint8_t read;
    I2C_read(ADDRESS, &read, 1, true);

    return read;
}

void io_expander_write_register(uint8_t reg, uint8_t value)
{
    const uint8_t write[] = { reg, value };
    I2C_write(ADDRESS, write, 2, true);
}

void io_expander_set_register_bit(uint8_t reg, uint8_t bit, bool value)
{
    uint8_t read = io_expander_read_register(reg);

    if (value)
        read |= bit;
    else    
        read &= ~bit;

    io_expander_write_register(reg, read);
}

bool io_expander_get_register_bit(uint8_t reg, uint8_t bit)
{
    return io_expander_read_register(reg) & bit;
}

void io_expander_set_pin_direction(uint8_t pin, bool direction)
{
    io_expander_set_register_bit(IO_DIRECTION, pin, direction);
}

void io_expander_set_output_value(uint8_t pin, bool value)
{
    io_expander_set_register_bit(OUTPUT_PORT_REGISTER, pin, value);
}

void io_expander_set_output_high_impedance(uint8_t pin, bool value)
{
    io_expander_set_register_bit(OUTPUT_HIGH_IMPEDANCE, pin, value);
}

void io_expander_set_input_default_state(uint8_t pin, bool state)
{
    io_expander_set_register_bit(INPUT_DEFAULT_STATE, pin, state);
}

void io_expander_enable_pullup_pulldown(uint8_t pin, bool enable)
{
    io_expander_set_register_bit(PULL_UP_PULL_DOWN_ENABLE, pin, enable);
}

void io_expander_pullup_pulldown_select(uint8_t pin, bool select)
{
    io_expander_set_register_bit(PULL_UP_PULL_DOWN_SELECT, pin, select);
}

void io_expander_enable_pin_irq(uint8_t pin, bool enable)
{
    io_expander_set_register_bit(INTERRUPT_MASK_REGISTER, pin, !enable);
}

bool io_expander_get_pin_status(uint8_t pin)
{
    return io_expander_get_register_bit(INPUT_STATUS_REGISTER, pin);
}

bool io_expander_get_pin_irq_status(uint8_t pin)
{
    return io_expander_get_register_bit(INTERRUPT_STATUS_REGISTER, pin);
}

void io_expander_acknowledge_interrupt()
{
    (void)io_expander_read_register(INTERRUPT_STATUS_REGISTER);
}

uint8_t io_expander_get_interrupt_register()
{
    return io_expander_read_register(INTERRUPT_STATUS_REGISTER);
}

bool io_expander_get_pin_irq_state(uint8_t pin)
{
    return io_expander_get_register_bit(INTERRUPT_STATUS_REGISTER, pin);
}