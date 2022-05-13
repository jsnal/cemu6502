#include "machine.h"
#include "handlers.h"
#include "utils.h"
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

int (**machine_initialize_handlers())(handler_params_t*)
{
  int (**handlers)(handler_params_t*) =
    (int (**)(handler_params_t*)) malloc(sizeof(int (*)(handler_params_t*)) * 0xFF + 1);

  handlers[0xA0] = handlers[0xA2] = handlers[0xA9] = handler_ld_imm;
  handlers[0xA5] = handlers[0xA6] = handlers[0xA4] = handler_ld_zpg;
  handlers[0x84] = handlers[0x85] = handlers[0x86] = handler_st_zpg;

  return handlers;
}

int machine_load(machine_t *machine, const uint8_t program[], size_t length, uint16_t start)
{
  assert_or_fatal(machine != NULL);

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
  assert_or_fatal(machine != NULL);

  while (!at_program_end(machine)) {
    uint8_t opcode = memory_get_next_byte(machine->memory, machine->cpu);
    int (*handler)(handler_params_t*) = handler_get(machine->handlers, opcode);

    assert_or_fatal(handler != NULL);

    handler_params_t *params = handler_get_params(machine, opcode);
    handler(params);
    free(params);
    /* cpu_registers_to_string(machine->cpu); */
  }

  cpu_registers_to_string(machine->cpu);
  return 0;
}
