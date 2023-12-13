from melopero_sensei import *
from time import sleep

sensei=MeloperoSensei()

sensei.set_text_font(TEXT_VERY_SMALL, TEXT_MONO)
sensei.imu_init()

while True:
    sensei.clear_display()
    
    acc=sensei.get_acceleration()
    rot=sensei.get_rotation()
    
    sensei.write_text("acceler.", 0, 20)
    sensei.write_text("gyroscope", 130, 20)
    
    sensei.write_text(str(acc[0]), 0, 60)
    sensei.write_text(str(acc[1]), 0, 80)
    sensei.write_text(str(acc[2]), 0, 100)
    
    sensei.write_text(str(rot[0]), 130, 60)
    sensei.write_text(str(rot[1]), 130, 80)
    sensei.write_text(str(rot[2]), 130, 100)
    
    sensei.update_display()
   
    sleep(1)
        