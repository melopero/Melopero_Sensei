#include <stdint.h>

typedef struct note note;

void FIFO_init();
void FIFO_insert(note c);
note FIFO_get();
uint8_t FIFO_is_empty(); 
uint8_t FIFO_is_full(); 
