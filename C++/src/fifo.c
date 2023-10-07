#include "fifo.h"
#include "audio.h"
#include <stdlib.h>

struct FIFO
{
    note *buffer;
    uint32_t head;
    uint32_t tail;
    uint32_t size;
};

FIFO *FIFO_new(uint32_t size)
{
    FIFO *fifo = (FIFO*)malloc(sizeof(FIFO));
    fifo->buffer = (note*)malloc(size * sizeof(note));
    fifo->size = size;

    FIFO_init(fifo);

    return fifo;
}

void FIFO_destroy(FIFO *fifo)
{
    free(fifo->buffer);
    free(fifo);
}

void FIFO_init(FIFO *fifo)
{
    fifo->head = 0;
    fifo->tail = 0;
} 

void FIFO_insert(FIFO *fifo, note n)
{
    if (!FIFO_is_full(fifo))
    {
        fifo->buffer[fifo->tail++] = n;
        fifo->tail %= fifo->size;
    }
}
note FIFO_get(FIFO *fifo)
{
    if (!FIFO_is_empty(fifo))
    {
        note n = fifo->buffer[fifo->head++];
        fifo->head %= fifo->size;

        return n;
    }
    else
    {
        note empty_note = {0.0f, 0.0f, 0.0f, false, 0.0f, 0};
        return empty_note;
    }
}

uint8_t FIFO_is_empty(FIFO *fifo) 
{
    return fifo->head == fifo->tail;
}

uint8_t FIFO_is_full(FIFO *fifo) 
{
    return (fifo->tail + 1) % fifo->size == fifo->head;
}