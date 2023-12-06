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


while True:
    sensei.clear_display()
    if(interrupt_flag is 1):
        sensei.play_note(600, 300, 1, False, 100)
        touch=sensei.get_touch()
        sensei.write_text(bin(touch), 20, 50)
        sensei.update_display()
        interrupt_flag=0
        sleep(0.2)
        
    
    

    

