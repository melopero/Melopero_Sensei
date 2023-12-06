from melopero_sensei import *
from time import sleep


sensei = MeloperoSensei()

sensei.set_display_color(180,0,0)
sensei.update_display()

sleep(2)

sensei.set_display_color(0,180,0)

sensei.set_text_color(0,0,255)
sensei.set_text_font(TEXT_MONO_BOLD, TEXT_BIG)

sensei.write_text("Hello", 50,130)

sensei.update_display()

sleep(2)

sensei.clear_display()
sensei.update_display()


