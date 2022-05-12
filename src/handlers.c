#include "handlers.h"
#include "machine.h"
#include "memory.h"
#include <stddef.h>

int (*handler_get(int (**handlers)(handler_params_t*), uint8_t opcode))(handler_params_t*)
{
  if (opcode < 0 || opcode > 0xFF) {
    return NULL;
  }

  return handlers[opcode];
}

// TODO: This isn't the best way to pass in parameters but it allows me to reuse
//       common code among certain instructions. It would be nice to have a better
//       way of implementing instructions that have similar implementation.
handler_params_t *handler_get_params(machine_t *machine, uint8_t opcode)
{
  handler_params_t *params = (handler_params_t*) malloc(sizeof(handler_params_t));
  params->cpu = machine->cpu;
  params->memory = machine->memory;
  params->reg1 = params->reg2 = NULL;

  switch (opcode) {
    case 0xA0: params->reg1 = &machine->cpu->y; break;
    case 0xA2: params->reg1 = &machine->cpu->x; break;
    case 0xA9: params->reg1 = &machine->cpu->a; break;
  }

  return params;
}

#include "utils.h"

int handler_ld_imm(handler_params_t *params)
{
  assert_or_fatal(params->reg1 != NULL && params->reg2 == NULL);
  *params->reg1 = memory_get_next_byte(params->memory, params->cpu);
  return 0;
}

