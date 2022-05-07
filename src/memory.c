#include "memory.h"
#include <stdlib.h>

memory_t *memory_create()
{
  memory_t *memory = (memory_t*) malloc(sizeof(memory_t));
  return memory;
}

void memory_set_byte(memory_t *memory, uint16_t index, uint8_t value)
{
  memory->blocks[index] = value;
}

uint8_t memory_get_byte(memory_t *memory, uint16_t index)
{
  return memory->blocks[index];
}
