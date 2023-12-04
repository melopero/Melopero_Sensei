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
        
    
    

    

