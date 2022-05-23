#include "machine.h"
#include "handlers.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static bool at_program_end(const machine_t *machine)
{
  return machine->cpu->pc >= machine->program_start + machine->program_size;
}

machine_t *machine_create()
{
  machine_t *machine = (machine_t*) malloc(sizeof(machine_t));
  machine->cpu = cpu_create();
  machine->memory = memory_create();
  machine->program_start = 0;

  return machine;
}

void machine_reset(machine_t *machine)
{
  assert_or_fatal(machine);

  machine->cpu->ps = machine->cpu->a = machine->cpu->x = machine->cpu->y = 0;
  machine->cpu->pc = machine->program_start;
  machine->cpu->sp = 0xFF;
  memset(machine->memory, 0, BLOCK_SIZE);
}

int machine_load(machine_t *machine, const uint8_t program[], size_t length, uint16_t start)
{
  assert_or_fatal(machine);

  for (size_t i = 0; i < length; i++) {
    memory_set_byte(machine->memory, start + i, program[i]);
  }

  machine->cpu->pc = start;
  machine->program_start = start;
  machine->program_size = length;
  return 0;
}

int machine_execute(machine_t *machine)
{
  assert_or_fatal(machine);
  handler_params_t params = { .machine = machine };

  while (!at_program_end(machine)) {
    uint8_t opcode = memory_get_next_byte(machine->memory, machine->cpu);
    void (*handler)(handler_params_t*) = handler_get(opcode);

    assert_or_fatal(handler);

    handler_get_params(&params, machine, opcode);
    handler(&params);
  }

  cpu_registers_to_string(machine->cpu);
  return 0;
}
