#include "analog.h"
#include "hardware/adc.h"
#include <math.h>

#define BATTERY_PIN                   26
#define LIGHT_PIN                     27
#define TEMPERATURE_IN                 4
#define ADC_GPIO_BASE                 26
#define ADC_SAMPLE_BIT_MASK       0x0FFF
#define ADC_SAMPLE_LEVELS           4095
#define ADC_VREF                     3.3

// ADC 48MHz default clock, 96 cycles per sample = 500 ks/s (free running mode)

void analog_init()
{
    adc_init();
    adc_gpio_init(BATTERY_PIN);
    adc_gpio_init(LIGHT_PIN);
    adc_set_temp_sensor_enabled(true);
}

/**** battery level sensor ****/

#define BATTERY_FULL     1.4
#define BATTERY_EMPTY    0.9

uint8_t analog_read_battery()
{
    adc_select_input(BATTERY_PIN - ADC_GPIO_BASE);
    
    uint16_t battery_raw = adc_read();
    float battery_v = (float)battery_raw / ADC_SAMPLE_LEVELS * ADC_VREF;
    
    float battery_level = (battery_v - BATTERY_EMPTY) / (BATTERY_FULL - BATTERY_EMPTY);
    battery_level *= 100;  
    if (battery_level > 100)
        battery_level = 100;

    return battery_level;
}

/**** light level sensor ****/

#define LIGHT_V_SAT    0.4
#define LIGHT_VCC      3.3

static uint16_t light_min_v = 0;
static uint16_t light_max_v = 3.3;

void analog_set_light_min()
{
    adc_select_input(LIGHT_PIN - ADC_GPIO_BASE);

    uint16_t light_min_raw = adc_read();
    light_min_v = (float)light_min_raw / ADC_SAMPLE_LEVELS * ADC_VREF;
}

void analog_set_light_max()
{
    adc_select_input(LIGHT_PIN - ADC_GPIO_BASE);
    
    uint16_t light_max_raw = adc_read();
    light_max_v = (float)light_max_raw / ADC_SAMPLE_LEVELS * ADC_VREF;
}

uint8_t analog_read_light()
{
    adc_select_input(LIGHT_PIN - ADC_GPIO_BASE);

    uint16_t light_raw = adc_read();
    float light_v = (float)light_raw / ADC_SAMPLE_LEVELS * ADC_VREF;

    float light_level = (light_v - light_min_v) / (light_max_v - light_min_v);
    light_level *= 100;
    if (light_level > 100)
        light_level = 100;

    return light_level;
}

uint16_t analog_read_light_raw()
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

    uint16_t temp_raw = adc_read();     
    float temp_v = (float)temp_raw / ADC_SAMPLE_LEVELS * ADC_VREF;   
    float temp_f = TEMP_REF - (temp_v - VBE_REF) / SENSOR_SLOPE;
    uint8_t temp_i = temp_f * 10;
    temp_i -= temp_i % 5;
    temp_f = (float)temp_i / 10;

    return temp_f;
}