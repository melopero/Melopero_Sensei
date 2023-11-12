#include <stdint.h>

void analog_init();
uint16_t analog_read_light();
uint8_t analog_read_battery();
float analog_read_temperature();