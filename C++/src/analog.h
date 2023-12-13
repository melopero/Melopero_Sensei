#include <stdint.h>

void analog_init();
void analog_set_light_min();
void analog_set_light_max();
uint8_t analog_read_light();
uint16_t analog_read_light_raw();
float analog_read_battery();
float percentage_read_battery(float voltage);
float analog_read_temperature();