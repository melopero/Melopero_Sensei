#include "PWM.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"

#include "pico/time.h"

static PWM_config config;

static uint32_t sys_clk_freq;
static uint16_t frequency;
static uint16_t compare_value;

void PWM_init(PWM_config *config_)
{
    config = *config_;

    pwm_config c = pwm_get_default_config();
    pwm_init(config.slice, &c, false);

    PWM_set_frequency(0.0f);                        
    PWM_set_compare_value(0);  
    PWM_set_top_value(0xFFFF);
    
    gpio_set_function(config.pin, GPIO_FUNC_PWM);

    sys_clk_freq = clock_get_hz(clk_sys);

    pwm_set_enabled(config.slice, true);
}

// set PWM clock divider for one slice (frequency)
void PWM_set_frequency(float value)
{
    frequency = value;

    float divider = sys_clk_freq / compare_value / value;
    pwm_set_clkdiv(config.slice, divider);
}

// set counter compare for one channel of one slice (duty cycle)
void PWM_set_compare_value(uint16_t value)
{
    compare_value = value;
    
    pwm_set_chan_level(config.slice, config.channel, value);

    PWM_set_frequency(frequency);
}

// set TOP value before wrap (frequency)
void PWM_set_top_value(uint16_t value)
{
    pwm_set_wrap(config.slice, value);    
}