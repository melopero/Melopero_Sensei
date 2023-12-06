from melopero_sensei import *
from time import sleep

sensei=MeloperoSensei()

sensei.set_text_font(TEXT_SANS_BOLD, TEXT_REGULAR)

while True:
    sleep(.1)
    sensei.clear_display()
    sensei.update_display()
    up = sensei.is_button_pressed(JOYSTICK_UP)
    left = sensei.is_button_pressed(JOYSTICK_LEFT)
    right = sensei.is_button_pressed(JOYSTICK_RIGHT)
    down = sensei.is_button_pressed(JOYSTICK_DOWN)
    center = sensei.is_button_pressed(JOYSTICK_CENTER)
    A = sensei.is_button_pressed(BUTTON_A)
    B = sensei.is_button_pressed(BUTTON_B)
    
    if(up is True):
        sensei.write_text("UP",100,50)      
    if(left is True):
        sensei.write_text("LEFT",10,100)
    if(right is True):
        sensei.write_text("RIGHT",100,100)
    if(down is True):
        sensei.write_text("DOWN",70,130)       
    if(center is True):
        sensei.write_text("CENTER",50,100)       
    if(A is True):
        sensei.write_text("A",80,200)    
    if(B is True):
        sensei.write_text("B",120,200)
        
        
    sensei.update_display()