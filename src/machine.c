#include "machine.h"
#include <stdlib.h>
#include <stdio.h>

machine_t *machine_create()
{
  machine_t *machine = (machine_t*) malloc(sizeof(machine_t));
  machine->cpu = cpu_create();
  machine->memory = memory_create();
  machine->program_start = 0;

  return machine;
}

int machine_load(machine_t *machine, const uint8_t program[], size_t length, uint16_t start)
{
  if (machine == NULL) {
    // TODO: Add error messages
    return -1;
  }

  for (size_t i = 0; i < length; i++) {
    memory_set_byte(machine->memory, start + i, program[i]);
  }

  machine->cpu->pc = start;
  machine->program_start = start;
  machine->program_size = length;
  return 0;
}

bool at_program_end(const machine_t *machine)
{
  return machine->cpu->pc >= machine->program_start + machine->program_size;
}

int machine_execute(machine_t *machine)
{
  while (!at_program_end(machine)) {
    uint8_t byte = memory_get_byte(machine->memory, machine->cpu->pc);

    printf("next byte: %02x\n", byte);
    machine->cpu->pc++;
  }

  return 0;
}
