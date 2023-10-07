#ifndef PWM_H
#define PWM_H

#include <stdint.h>

typedef struct PWM_config
{
    uint8_t slice;
    uint8_t channel;
    uint8_t pin;
} PWM_config;

void PWM_init(PWM_config *config_);
void PWM_set_frequency(float frequency);
void PWM_set_compare_value(uint16_t value);
void PWM_set_top_value(uint16_t value);

#endif  // PWM_H