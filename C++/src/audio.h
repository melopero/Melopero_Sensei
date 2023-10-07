#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>
#include <stdbool.h>
#include "notes.h"

typedef struct note
{
    float frequency;
    uint32_t duration;
    float volume;
    bool sweep_direction;
    float sweep_time;
    uint32_t elapsed;
} note;

void audio_init(void);
void audio_deinit(void);
void audio_play_note(float frequency, uint32_t duration, float volume, bool sweep_direction, float sweep_time);

#endif  // AUDIO_H