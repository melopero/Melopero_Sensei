#include "input.h"
#include "io_expander.h"
#include "hardware/gpio.h"

#define INT_PIN    4

extern void gpio_irq_callback(uint gpio, uint32_t event_mask);
//static bool input_get_button(uint8_t button);

void input_init(void)
{
    io_expander_init();
    
    io_expander_set_pin_direction(RIGHT, PIN_IN);
    io_expander_set_pin_direction(LEFT, PIN_IN);
    io_expander_set_pin_direction(UP, PIN_IN);
    io_expander_set_pin_direction(DOWN, PIN_IN);
    io_expander_set_pin_direction(CENTER, PIN_IN);
    io_expander_set_pin_direction(A, PIN_IN);
    io_expander_set_pin_direction(B, PIN_IN);
    io_expander_set_pin_direction(NONE, PIN_OUT);

    io_expander_set_output_high_impedance(NONE, PIN_HIGH);

    io_expander_pullup_pulldown_select(RIGHT, PULL_UP);
    io_expander_pullup_pulldown_select(LEFT, PULL_UP);
    io_expander_pullup_pulldown_select(UP, PULL_UP);
    io_expander_pullup_pulldown_select(DOWN, PULL_UP);
    io_expander_pullup_pulldown_select(CENTER, PULL_UP);
    io_expander_pullup_pulldown_select(A, PULL_UP);
    io_expander_pullup_pulldown_select(B, PULL_UP);

    io_expander_enable_pullup_pulldown(RIGHT, ENABLE);
    io_expander_enable_pullup_pulldown(LEFT, ENABLE);
    io_expander_enable_pullup_pulldown(UP, ENABLE);
    io_expander_enable_pullup_pulldown(DOWN, ENABLE);
    io_expander_enable_pullup_pulldown(CENTER, ENABLE);
    io_expander_enable_pullup_pulldown(A, ENABLE);
    io_expander_enable_pullup_pulldown(B, ENABLE);

    io_expander_set_input_default_state(RIGHT, PIN_HIGH);
    io_expander_set_input_default_state(LEFT, PIN_HIGH);
    io_expander_set_input_default_state(UP, PIN_HIGH);
    io_expander_set_input_default_state(DOWN, PIN_HIGH);
    io_expander_set_input_default_state(CENTER, PIN_HIGH);
    io_expander_set_input_default_state(A, PIN_HIGH);
    io_expander_set_input_default_state(B, PIN_HIGH);

    io_expander_enable_pin_irq(RIGHT, ENABLE);
    io_expander_enable_pin_irq(LEFT, ENABLE);
    io_expander_enable_pin_irq(UP, ENABLE);
    io_expander_enable_pin_irq(DOWN, ENABLE);
    io_expander_enable_pin_irq(CENTER, ENABLE);
    io_expander_enable_pin_irq(A, ENABLE);
    io_expander_enable_pin_irq(B, ENABLE);
}

ButtonState input_get_button_state(uint8_t button)
{
    static bool leftButtonStateOld = false;
    static bool rightButtonStateOld = false;
    static bool upButtonStateOld = false;
    static bool downButtonStateOld = false;
    static bool centerButtonStateOld = false;
    static bool AButtonStateOld = false;
    static bool BButtonStateOld = false;

    bool buttonStateCurrent = false, buttonStateOld = false;

    switch (button)
    {
        case LEFT:
            buttonStateCurrent = input_get_button(LEFT);
            buttonStateOld = leftButtonStateOld;
            leftButtonStateOld = buttonStateCurrent;
            break;

        case RIGHT:
            buttonStateCurrent = input_get_button(RIGHT);
            buttonStateOld = rightButtonStateOld;
            rightButtonStateOld = buttonStateCurrent;
            break;

        case UP:
            buttonStateCurrent = input_get_button(UP);
            buttonStateOld = upButtonStateOld;
            upButtonStateOld = buttonStateCurrent;
            break;

        case DOWN:
            buttonStateCurrent = input_get_button(DOWN);
            buttonStateOld = downButtonStateOld;
            downButtonStateOld = buttonStateCurrent;
            break;

        case CENTER:
            buttonStateCurrent = input_get_button(CENTER);
            buttonStateOld = centerButtonStateOld;
            centerButtonStateOld = buttonStateCurrent;
            break;

        case A:
            buttonStateCurrent = input_get_button(A);
            buttonStateOld = AButtonStateOld;
            AButtonStateOld = buttonStateCurrent;
            break;

        case B:
            buttonStateCurrent = input_get_button(B);
            buttonStateOld = BButtonStateOld;
            BButtonStateOld = buttonStateCurrent;
            break;
    }

    if (buttonStateCurrent)
        if (buttonStateOld)
            return PRESSED;
        else
            return JUST_PRESSED;
    else // !buttonStateCurrent  
        if (buttonStateOld)
            return JUST_RELEASED;
        else
            return RELEASED;
}

bool input_is_pressed(uint8_t button)
{
    return input_get_button_state(button) == JUST_PRESSED || input_get_button_state(button) == PRESSED;
}

bool input_get_button(uint8_t button)
{
    bool status = io_expander_get_pin_status(button);
    return !status;
}

uint8_t input_get_interrupt_register()
{
    return io_expander_get_interrupt_register();
}

bool input_get_button_irq_state(uint8_t button)
{
    return io_expander_get_pin_irq_state(button);
}

void input_enable_interrupt(bool enable)
{
    if (enable)
    {
        gpio_init(INT_PIN);
        gpio_set_dir(INT_PIN, false);
        gpio_set_pulls(INT_PIN, false, false);
        gpio_set_irq_enabled_with_callback(INT_PIN, GPIO_IRQ_EDGE_FALL, true, gpio_irq_callback);
        io_expander_acknowledge_interrupt();  
    }
    else
        gpio_set_irq_enabled_with_callback(INT_PIN, GPIO_IRQ_EDGE_FALL, false, gpio_irq_callback);
}

// note: use either IRQ or polling (race condition - I2C shared resource)
void gpio_irq_callback(uint gpio, uint32_t event_mask)
{
    io_expander_acknowledge_interrupt();
}

