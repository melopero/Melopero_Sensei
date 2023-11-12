#include "analog.h"
#include "hardware/adc.h"
#include <math.h>

#define BATTERY_PIN       26
#define LIGHT_PIN         27
#define TEMPERATURE_IN     4
#define ADC_GPIO_BASE     26

#define SAMPLE_BITS       12
#define SAMPLE_LEVELS   4096
#define ADC_VREF         3.3

// ADC 48MHz default clock, 96 cycles per sample = 500 ks/s (free running mode)

void analog_init()
{
    adc_init();
    adc_gpio_init(BATTERY_PIN);
    adc_gpio_init(LIGHT_PIN);
    adc_set_temp_sensor_enabled(true);
}

/**** battery level sensor ****/

#define BATTERY_FULL     1.3
#define BATTERY_EMPTY    0.0

uint8_t analog_read_battery()
{
    adc_select_input(BATTERY_PIN - ADC_GPIO_BASE);
    
    uint16_t battery_raw = adc_read() & 0x0FFF;
    float battery_v = (float)battery_raw / SAMPLE_LEVELS * ADC_VREF;
    float battery_level = (battery_v - BATTERY_EMPTY) / (BATTERY_FULL - BATTERY_EMPTY);

    battery_level = battery_level * 100;  // battery level is 0 - 100
    if (battery_level > 100)
        battery_level = 100;

    return battery_level;
}

/**** light level sensor ****/

uint16_t analog_read_light()
{
    adc_select_input(LIGHT_PIN - ADC_GPIO_BASE);

    return adc_read();
}

/**** temperature sensor ****/

#define VBE_REF                0.706
#define TEMP_REF                  24
#define SENSOR_SLOPE       -1.721e-3

float analog_read_temperature()
{
    adc_select_input(TEMPERATURE_IN);

    uint16_t temp_raw = adc_read() & 0x0FFF;
    float temp_v = (float)temp_raw / SAMPLE_LEVELS * ADC_VREF;
    float temp_f = TEMP_REF - (temp_v - VBE_REF) / SENSOR_SLOPE;
    uint8_t temp_i = temp_f * 10;
    temp_i -= temp_i % 5;
    temp_f = (float)temp_i / 10;

    return temp_f;
}