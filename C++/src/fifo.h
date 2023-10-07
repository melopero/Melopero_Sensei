#include <stdint.h>

typedef struct FIFO FIFO;
typedef struct note note;

FIFO *FIFO_new(uint32_t size);
void FIFO_destroy(FIFO *fifo);

void FIFO_init(FIFO *fifo);
void FIFO_insert(FIFO *fifo, note c);
note FIFO_get(FIFO *fifo);
uint8_t FIFO_is_empty(FIFO *fifo); 
uint8_t FIFO_is_full(FIFO *fifo); 
