#include "machine.h"
#include "handlers.h"
#include <stdlib.h>
#include <stdio.h>

static bool at_program_end(const machine_t *machine)
{
  return machine->cpu->pc >= machine->program_start + machine->program_size;
}

machine_t *machine_create()
{
  machine_t *machine = (machine_t*) malloc(sizeof(machine_t));
  machine->handlers = machine_initialize_handlers();
  machine->cpu = cpu_create();
  machine->memory = memory_create();
  machine->program_start = 0;

  return machine;
}

int (**machine_initialize_handlers())(machine_t*)
{
  int (**handlers)(machine_t*) = (int (**)(machine_t*)) malloc(sizeof(int (*)(machine_t*)) * 0xFF + 1);

  handlers[0xA0] = handler_ldy_imm;
  handlers[0xA2] = handler_ldx_imm;
  handlers[0xA9] = handler_lda_imm;

  return handlers;
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

int machine_execute(machine_t *machine)
{
  while (!at_program_end(machine)) {
    uint8_t byte = memory_get_next_byte(machine->memory, machine->cpu);
    int (*handler)(machine_t*) = handler_get(machine->handlers, byte);

    if (handler != NULL) {
      handler(machine);
    }
  }

  cpu_registers_to_string(machine->cpu);
  return 0;
}
