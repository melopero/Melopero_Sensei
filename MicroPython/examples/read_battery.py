from melopero_sensei import *
from time import sleep

sensei=MeloperoSensei()

sensei.set_text_font(TEXT_BIG, TEXT_SANS_BOLD)

while True:
    sensei.clear_display()
    
    bat = sensei.read_battery()
    
    sensei.write_text("BAT", 60,80)
    sensei.write_text(str(bat), 60,150)
    
    sensei.update_display()
    sleep(1)
    
