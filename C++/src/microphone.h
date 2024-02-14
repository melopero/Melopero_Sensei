#ifndef MICROPHONE_H
#define MICROPHONE_H

#include <stdbool.h>
#include <stdint.h>

extern volatile int samples_read;
extern int16_t sample_buffer[];

void microphone_init(void);
void microphone_deinit(void);
void microphone_enable(bool enalble);

#endif  // MICROPHONE_H