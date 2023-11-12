#include <stdint.h>

void analog_init();
void analog_set_light_min();
void analog_set_light_max();
uint8_t analog_read_light();
uint16_t analog_read_light_raw();
uint8_t analog_read_battery();
float analog_read_temperature();