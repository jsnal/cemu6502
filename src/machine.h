#ifndef MACHINE_H
#define MACHINE_H

#include "cpu.h"
#include "memory.h"
#include <stdlib.h>

#define A machine->cpu->a
#define X machine->cpu->x
#define Y machine->cpu->y

typedef struct machine_struct {
  cpu_t *cpu;
  memory_t *memory;
  uint16_t program_start;
  uint16_t program_size;
} machine_t;

machine_t *machine_create();
int machine_load(machine_t*, const uint8_t program[], size_t length, uint16_t start);
int machine_execute(machine_t*);

#endif
