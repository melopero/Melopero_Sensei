#ifndef IO_EXPANDER_H
#define IO_EXPANDER_H

#include "I2C.h"

#define P0                        (1 << 0)
#define P1                        (1 << 1)
#define P2                        (1 << 2)
#define P3                        (1 << 3)
#define P4                        (1 << 4)
#define P5                        (1 << 5)
#define P6                        (1 << 6)
#define P7                        (1 << 7)

#define PIN_OUT                         1
#define PIN_IN                          0

#define PIN_HIGH                        1
#define PIN_LOW                         0

#define PULL_UP                         1
#define PULL_DOWN                       0

#define ENABLE                          1
#define DISABLE                         0

void io_expander_init();

void io_expander_set_pin_direction(uint8_t pin, bool direction);
void io_expander_set_output_value(uint8_t pin, bool value);
void io_expander_set_output_high_impedance(uint8_t pin, bool value);
void io_expander_set_input_default_state(uint8_t pin, bool state);
void io_expander_enable_pullup_pulldown(uint8_t pin, bool enable);
void io_expander_pullup_pulldown_select(uint8_t pin, bool select);
void io_expander_enable_pin_irq(uint8_t pin, bool enable);

bool io_expander_get_pin_status(uint8_t pin);
bool io_expander_get_pin_irq_status(uint8_t pin);
void io_expander_acknowledge_interrupt();

#endif  // IO_EXPANDER_H