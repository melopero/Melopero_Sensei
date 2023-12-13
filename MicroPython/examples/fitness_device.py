from melopero_sensei import *
from time import sleep

sensei=MeloperoSensei()

sensei.set_text_font(TEXT_BIG, TEXT_MONO_BOLD)
sensei.imu_init()

sensei.enable_pedometer(True)
sensei.reset_steps()

while True:
    sensei.clear_display()
    sensei.update_display()
    
    steps = sensei.get_steps()
    calories = 0.05 * steps
    
    sensei.write_text("steps: "+ str(steps), 70, 100)
    sensei.write_text("cal: "+ str(calories), 70, 150)
    
    sensei.update_display()
    sleep(1)
    
    
