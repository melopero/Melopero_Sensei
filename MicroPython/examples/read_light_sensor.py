from melopero_sensei import *
from time import sleep

sensei=MeloperoSensei()

sensei.set_text_font(TEXT_BIG, TEXT_SANS_BOLD)

while True:
    sensei.clear_display()
    
    light = sensei.read_light()
    
    sensei.write_text("LIGHT", 50,80)
    sensei.write_text(str(light), 60,150)
    
    sensei.update_display()
    sleep(1)
    
