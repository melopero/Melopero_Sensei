from melopero_sensei import *
from time import sleep

sensei=MeloperoSensei()

sensei.set_text_font(TEXT_VERY_SMALL, TEXT_MONO)
sensei.imu_init()

while True:
    sensei.clear_display()
    sensei.update_display()
    
    freefall = sensei.get_freefall()
    single = sensei.get_single_tap()
    
    if freefall:
        sensei.write_text("Freefall", 30, 150)
        sensei.update_display()
        sleep(1)
        
    
    if single:
        sensei.write_text("Single", 50, 150)
        sensei.update_display()
        sleep(1)
    
    
   
    
        
