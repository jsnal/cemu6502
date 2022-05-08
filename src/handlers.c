#include "handlers.h"
#include "memory.h"

int handler_lda_imm(machine_t *machine)
{
  machine->cpu->a = memory_get_next_byte(machine->memory, machine->cpu);
  return 0;
}

int handler_ldx_imm(machine_t *machine)
{
  machine->cpu->x = memory_get_next_byte(machine->memory, machine->cpu);
  return 0;
}

int handler_ldy_imm(machine_t *machine)
{
  machine->cpu->y = memory_get_next_byte(machine->memory, machine->cpu);
  return 0;
}
