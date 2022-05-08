#include "handlers.h"
#include "memory.h"

int (*handler_get(int (**handlers)(machine_t*), uint8_t opcode))(machine_t*)
{
  if (opcode < 0 || opcode > 0xFF) {
    return NULL;
  }

  return handlers[opcode];
}

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
