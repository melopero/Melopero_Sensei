#ifndef MICROPHONE_H
#define MICROPHONE_H

#include <stdbool.h>
#include <stdint.h>

extern volatile int samples_read;
extern int16_t sample_buffer[];

void microphone_init(void);
void microphone_deinit(void);
void microphone_enable(bool enable);
int microphone_get_samples_read(void);
int16_t microphone_get_sample(int index);

#endif  // MICROPHONE_H