#include "audio_fifo.h"
#include "audio.h"
#include <stdlib.h>

#define AUDIO_BUFFER_SIZE  100

static note buffer[100];
static uint32_t head;
static uint32_t tail;
static uint32_t size;

void FIFO_init()
{
    head = 0;
    tail = 0;
} 

void FIFO_insert(note n)
{
    if (!FIFO_is_full())
    {
        buffer[tail++] = n;
        tail %= size;
    }
}
note FIFO_get()
{
    if (!FIFO_is_empty())
    {
        note n = buffer[head++];
        head %= size;

        return n;
    }
    else
    {
        note empty_note = {0.0f, 0.0f, 0.0f, false, 0.0f, 0};
        return empty_note;
    }
}

uint8_t FIFO_is_empty() 
{
    return head == tail;
}

uint8_t FIFO_is_full() 
{
    return (tail + 1) % size == head;
}