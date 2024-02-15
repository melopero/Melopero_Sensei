extern "C"
{
#include "../../src/microphone.h"
}

#include "../../src/MeloperoSensei.hpp"
#include <string>

int main(int argc, char *argv[])
{
    MeloperoSensei sensei;

    sensei.microphoneInit();
    sensei.microphoneEnable(true);
    
    while (true)
    {
        int numSamples;

        while ((numSamples = sensei.microphoneGetNumSamples()) == 0)
            ;

        int16_t avg = 0;
        for (int i = 0; i < numSamples; i++)
            avg += sensei.microphoneGetSample(i);

        avg /= numSamples;

        sensei.clearScreen(0x00, 0x00, 0x00);
        sensei.print(10, 30 , std::to_string(avg));
        sensei.presentScreen();   
    }

    return 0;
}