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
    sensei.print(0, 0, "Initializing Touch...", 255, 255, 255);
    sensei.presentScreen();

    MPR121_reset();
    MPR121_run();

    sensei.clearScreen(0, 0, 0);
    sensei.print(0, 0, "Touch Initalized", 255, 255, 255);
    sensei.presentScreen();
}
