#include <stdio.h>
#include "pico/stdlib.h"
#include "../../src/MeloperoSensei.hpp"

extern "C"
{
#include "../../src/MPR121.h"
}

int main()
{

    MeloperoSensei sensei;
   
    sensei.setTextFont(FREE_SANS_BOLD, SMALL);
    sensei.setTextColor(0x93, 0x70, 0xDB);

    sensei.print(20, 100, "Init");
    sensei.presentScreen();

    MPR121_reset();
    uint8_t config2_reg_content;
    MPR121_read_reg(MPR121_CONFIG2_REGISTER, &config2_reg_content);

    sensei.clearScreen(0, 0, 0);
    if (config2_reg_content == 0x24)
        sensei.print(15, 100, "Touch Initialized");
    else
        sensei.print(15, 100, "Touch Initialization Failed");
    sensei.presentScreen();

    uint16_t touch_status = 0;

    while (true)
    {

        sensei.clearScreen(0, 0, 0);

        // Update touch status. The touch status tells us which electrodes are being touched.
        // The value should be different from 0 if at least one electrode is being touched.
        MPR121_get_touch_status(&touch_status);
        char buf[32];
        sprintf(buf, "%x", touch_status);
        sensei.print(20, 160, "touch status: ");
        sensei.print(20, 180, buf);

        uint8_t result = 0;
        MPR121_read_reg(MPR121_ELECTRODE_CONFIG_REGISTER, &result);
        char res[16];
        sprintf(res, "%x", result);
        sensei.print(20, 80, "elec config reg: ");
        sensei.print(20, 100, res);

        sensei.presentScreen();
    }
}