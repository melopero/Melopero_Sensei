# Melopero_Sensei

This repository is still in beta and is updated daily. It includes C/C++ source code and examples, a beta MicroPython port (source files, a compiled installer that includes the Sensei library, and some examples).

A PDF guide will be released very soon.

Beta MicroPython functions:

`MeloperoSensei()`
Create a new Sensei object

`update_display()`
This function is crucial to make the changes applied to the display effective. It should be called last after making the modifications you want to be displayed on the screen, including clearing the display.

`clear_display()`
Clear all the display shutting down all the pixels.
After this function remember to call update_display()

`set_display_color(red, green, blue)`
This function clears the entire display, setting the color defined by the red, green, and blue components. These components can have values ranging from 0 to 255.

`set_text_font(font,size)`
Set the font and text size. As for the font, you can pass one of the following options to the function: TEXT_MONO, TEXT_MONO_BOLD, TEXT_SANS, TEXT_SANS_BOLD. The adjustable sizes are TEXT_VERY_SMALL, TEXT_SMALL, TEXT_REGULAR, TEXT_BIG.

`set_text_color(red, green, blue)`
Set the font color by providing the red, green, and blue components as parameters. For each component, the allowed values range from 0 to 255.

`write_text(text,x,y)`
Writes text at a specified position on the screen (x, y). It takes as parameters the text and the coordinates (x, y) of the starting point. To set the font, size, and color of the text, it is necessary to call the corresponding functions first.

`draw_pixel(x, y, red, green, blue)`
Colors the pixel at the position (x, y). The color is defined by its red, green, and blue components, with values ranging from 0 to 255.

`draw_line(startx, starty, endx, endy, red, green, blue)`
Draws a line from the point with coordinates (startx, starty) to the point (endx, endy). The color is defined by its red, green, and blue components, with values ranging from 0 to 255.

`draw_rect(startx, starty, endx, endy, red, green, blue)`
Draws an empty rectangle starting from the point with coordinates (startx, starty) and extending to the point (endx, endy). The color of the rectangle is defined by its red, green, and blue components, with values ranging from 0 to 255.

`draw_fill_rect(startx, starty, endx, endy, red, green, blue)`
Draws a filled rectangle starting from the point with coordinates (startx, starty) and extending to the point (endx, endy). The color of the rectangle is defined by its red, green, and blue components, with values ranging from 0 to 255.

`draw_sprite(buffer, startx, starty, width, height)`
This function allows displaying a 240x240 image on the display or embedding a smaller image at a specific point. The image must be passed as input through a byte array, not directly as an image file. To create a byte array from an image, refer to the section "Displaying an Image on the Display."
The other parameters to pass to the function are the starting point (x, y) and the width and height of the image.

`get_cpu_temp()`
Returns the temperature of the RP2040 microcontroller


Beta functions (still buggy):

`is_button_pressed(button)`
Returns true or false, depending on the button status.
Button can be JOYSTICK_UP, JOYSTICK_DOWN, JOYSTICK_LEFT, JOYSTICK_RIGHT, JOYSTICK_CENTER, BUTTON_A, BUTTON_B.

`get_button_irq_state(button)`
The irq button state is 1 if the button has been pressed.
Every time this function is called, the values are cleared to 0.

`get_interrupt_register()`
There is an expander on the Sensei that manages the joystick and buttons, this register tells you if and which buttons have been pressed (set to 1) since the last time you called it. It is useful to control multiple inputs
 
`enable_button_interrupt(enable)`
The expander can send an interrupt on pin gpio4 the first time a button is pressed.
You can use the interrupt to call the function get_interrupt_register() and check which buttons have been pressed. Only the first interrupt is sent, then it must be cleared calling this function to re-trigger it.

`play_note(frequency, duration, volume, sweep_direction, sweep_time)`
example: sensei.play_note(600, 300, 1, False, 100)
   

`get_button_state(button)`
Returns the current button state

`read_battery()`
Returns the battery level

read_light()
Returns the light sensor level


The following two functions can be used to calibrate the light sensor:
`set_light_min()`
Set the light sensor minimum value

`set_light_max()`
Set the light sensor maximum value


`touch_init()`
Enable the touch sensor with all the 12 electrodes.

`get_touch()`
Returns 16bit number that represents the electrodes status. If the corresponding bit is 1, the electrode is pressed.


###Quick start: draw a line
```python
from melopero_sensei import *
from time import sleep

sensei=MeloperoSensei()

sensei.draw_line(10,10,60,60,100,20,50)
sensei.draw_line(60,60,200,60,0,255,0)
sensei.draw_pixel(100,100,255,255,0)
sensei.update_display()

```

###Quick start: enable the touch sensor and the buzzer
```python
from melopero_sensei import *
from time import sleep
from machine import Pin

interrupt_flag=0
pin = Pin(5,Pin.IN,Pin.PULL_UP)


def callback(pin):
    global interrupt_flag
    interrupt_flag=1
    sensei.write_text("interrupt", 20,150)

pin.irq(trigger=Pin.IRQ_FALLING, handler=callback)

sensei=MeloperoSensei()

sensei.set_text_font(TEXT_VERY_SMALL, TEXT_SANS)
sensei.touch_init()

#sensei.update_display()

while True:
    sensei.clear_screen()
    
    bat = sensei.read_battery()
    light = sensei.read_light()
    sensei.write_text(str(bat), 120,120)
    sensei.write_text(str(light), 120,160)
    reg=sensei.get_interrupt_register()
    sensei.write_text(bin(reg),20,180)
    sensei.update_display()
    

    if(interrupt_flag is 1):
        sensei.play_note(600, 300, 1, False, 100)
        touch=sensei.get_touch()
        sensei.write_text(bin(touch), 20, 50)
        sensei.set_display_color(255,000,0)
        sensei.update_display()
        interrupt_flag=0
        sleep(0.4)
```