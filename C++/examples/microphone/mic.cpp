extern "C"
{
#include "../../src/microphone.h"
}

#include "../../src/MeloperoSensei.hpp"
#include <string>

int main(int argc, char *argv[])
{
    MeloperoSensei sensei;

    microphone_init();
    microphone_enable(true);
    
    while (true)
    {
        while (samples_read == 0)
            ;

        int numSamples = samples_read;
        samples_read = 0;

        int16_t avg = 0;
        for (int i = 0; i < numSamples; i++)
            avg += sample_buffer[i];

        avg /= numSamples;

        sensei.clearScreen(0x00, 0x00, 0x00);
        sensei.print(10, 30 , std::to_string(avg));
        sensei.presentScreen();   
    }

    return 0;
}