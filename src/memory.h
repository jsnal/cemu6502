#ifndef MEMORY_H
#define MEMORY_H

#include "cpu.h"
#include <stdint.h>

#define BLOCK_SIZE 0xFFFF

typedef struct memory_struct {
  uint8_t blocks[BLOCK_SIZE];
} memory_t;

memory_t *memory_create();

void memory_set_byte(memory_t*, uint16_t index, uint8_t value);
uint8_t memory_get_byte(memory_t*, uint16_t index);
uint8_t memory_get_next_byte(memory_t*, cpu_t*);

#endif
