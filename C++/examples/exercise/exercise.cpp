#include "../../src/MeloperoSensei.hpp"
#include "heart.h"

int main()
{
    MeloperoSensei sensei;

    sensei.enableButtonInterrupt(true);

    while (true)
    {
        sensei.clearScreen(0x00, 0xAA, 0xFF);

        sensei.drawSprite(heart, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

        sensei.setTextFont(FREE_SANS_BOLD, REGULAR);
        sensei.setTextColor(0xFF, 0xFF, 0xFF);
        sensei.print(100, 120, "LD");

        sensei.presentScreen();
    }
}