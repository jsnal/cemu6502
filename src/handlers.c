#include "handlers.h"
#include "machine.h"
#include "memory.h"
#include "utils.h"
#include <stddef.h>

static void set_zero_and_negative_flags(cpu_t *cpu, uint8_t value)
{
  set_processor_status(cpu, PS_ZERO, value == 0);
  set_processor_status(cpu, PS_NEGATIVE, (value & 0x80) == 0x80);
}

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
    /* Store Zero Page */
    case 0x84: params->reg1 = &Y; break;
    case 0x85: params->reg1 = &A; break;
    case 0x86: params->reg1 = &X; break;

    /* Load Immediate */
    case 0xA0: params->reg1 = &Ymachine->cpu->y; break;
    case 0xA2: params->reg1 = &Xmachine->cpu->x; break;
    case 0xA4: params->reg1 = &Ymachine->cpu->y; break;

    /* Load Zero Page */
    case 0xA5: params->reg1 = &machine->cpu->a; break;
    case 0xA6: params->reg1 = &machine->cpu->x; break;
    case 0xA9: params->reg1 = &machine->cpu->a; break;

    /* Load Zero Page Indexed */
    case 0xB4:
      params->reg1 = &machine->cpu->y;
      params->reg2 = &machine->cpu->x;
      break;
    case 0xB5:
      params->reg1 = &machine->cpu->a;
      params->reg2 = &machine->cpu->x;
      break;
    case 0xB6:
      params->reg1 = &machine->cpu->x;
      params->reg2 = &machine->cpu->y;
      break;
  }

  return params;
}

int handler_ld_imm(handler_params_t *params)
{
  assert_or_fatal(params->reg1 != NULL && params->reg2 == NULL);
  *params->reg1 = memory_get_next_byte(params->memory, params->cpu);
  set_zero_and_negative_flags(params->cpu, *params->reg1);
  return 0;
}

int handler_ld_zpg(handler_params_t *params)
{
  assert_or_fatal(params->reg1 != NULL && params->reg2 == NULL);
  uint8_t address = memory_get_next_byte(params->memory, params->cpu);
  *params->reg1 = memory_get_byte(params->memory, address);
  set_zero_and_negative_flags(params->cpu, *params->reg1);
  return 0;
}

int handler_ld_zpg_idx(handler_params_t *params)
{
  assert_or_fatal(params->reg1 != NULL && params->reg2 != NULL);
  uint8_t address = memory_get_next_byte(params->memory, params->cpu);
  *params->reg1 = memory_get_byte(params->memory, address) + *params->reg2;
  set_zero_and_negative_flags(params->cpu, *params->reg1);
  return 0;
}

int handler_st_zpg(handler_params_t *params)
{
  assert_or_fatal(params->reg1 != NULL && params->reg2 == NULL);
  uint8_t address = memory_get_next_byte(params->memory, params->cpu);
  memory_set_byte(params->memory, address, *params->reg1);
  return 0;
}
