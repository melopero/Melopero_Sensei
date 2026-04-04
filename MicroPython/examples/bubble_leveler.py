# Bubble_leveler v1.0
# 01/04/2026 , by Agostino Rolando
#
from melopero_sensei import *
from machine import Pin
from time import sleep

sensei = MeloperoSensei()
sensei.imu_init()
sensei.set_text_font(TEXT_VERY_SMALL, TEXT_SANS)
sensei.touch_init()

def bubble_test():
    while True:
        sensei.clear_display()

        acc = sensei.get_acceleration()
        x = acc[0] / 10
        y = acc[1] / 10
        z = acc[2] / 10

        zref = 70
        dispcenter = 100
        
        if (abs(z) > zref):
            # orizz
            xx = int(abs(z))
            yy = int(x)
            zz = int(y)

            sensei.draw_rect(10,10,230,230,0,100,255)
            sensei.draw_rect(50,50,150,150,0,100,255)
            sensei.draw_rect(85,85,70,70,0,100,255)
            sensei.draw_line((dispcenter-40),(dispcenter+20),(dispcenter+80),(dispcenter+20),0,255,0)
            sensei.draw_line((dispcenter+20),(dispcenter-40),(dispcenter+20),(dispcenter+80),0,255,0)

            # 🔧 clamp per evitare uscita schermo
            px = max(0, min(240, (xx + yy)))
            py = max(0, min(240, (xx - zz)))

            sensei.draw_fill_rect(px, py, 30, 30, 0, 100, 255)

            sensei.update_display()   # ✅ FIX IMPORTANTE

        else:
            sensei.write_text("??", 20, 120)
            sensei.update_display()
            sleep(0.05)   # ✅ più fluido

# --- MAIN ---
bubble_test()