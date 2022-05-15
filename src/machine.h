#ifndef MACHINE_H
#define MACHINE_H

#include "cpu.h"
#include "memory.h"
#include <stdlib.h>

#define A machine->cpu->a
#define X machine->cpu->x
#define Y machine->cpu->y

typedef struct handler_params_struct {
  cpu_t *cpu;
  memory_t *memory;
  uint8_t *reg1;
  uint8_t *reg2;
} handler_params_t;

typedef struct machine_struct {
  cpu_t *cpu;
  memory_t *memory;
  void (**handlers)(handler_params_t*);
  uint16_t program_start;
  uint16_t program_size;
} machine_t;

machine_t *machine_create();
void (**(machine_initialize_handlers()))(handler_params_t*);
int machine_load(machine_t*, const uint8_t program[], size_t length, uint16_t start);
int machine_execute(machine_t*);

#endif
