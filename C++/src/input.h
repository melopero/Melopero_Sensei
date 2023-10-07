#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stdint.h>
#include "io_expander.h"

#define RIGHT       P4 
#define LEFT        P3
#define UP          P1
#define DOWN        P2
#define CENTER      P5
#define A           P7
#define B           P6
#define NONE        P0

typedef enum ButtonState 
{
    JUST_PRESSED, PRESSED, JUST_RELEASED, RELEASED,
} ButtonState;

void input_init(void);
ButtonState input_get_button_state(uint8_t button);

#endif  // INPUT_H