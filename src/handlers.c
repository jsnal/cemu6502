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

static uint8_t get_zpg_index_address(uint8_t address, uint8_t reg)
{
  return address + reg;
}

static uint16_t get_abs_address(memory_t *memory, cpu_t *cpu)
{
  uint8_t low = memory_get_next_byte(memory, cpu);
  uint8_t high = memory_get_next_byte(memory, cpu);
  return (high << 8) + low;
}

void (*handler_get(void (**handlers)(handler_params_t*), uint8_t opcode))(handler_params_t*)
{
  if (opcode < 0 || opcode > 0xFF) {
    return NULL;
  }

  return handlers[opcode];
}

// TODO: This isn't the best way to pass in parameters but it allows me to reuse
//       common code among certain instructions. It would be nice to have a better
//       way of implementing instructions that have similar implementation.
handler_params_t *handler_get_params(handler_params_t *params, machine_t *machine, uint8_t opcode)
{
  REG1 = REG2 = NULL;

  switch (opcode) {
    /* Store Zero Page */
    case 0x84: REG1 = &Y; break;
    case 0x85: REG1 = &A; break;
    case 0x86: REG1 = &X; break;

    case 0x8C: REG1 = &Y; break;
    case 0x8D: REG1 = &A; break;
    case 0x8E: REG1 = &X; break;

    /* Load Immediate */
    case 0xA0: REG1 = &Y; break;
    case 0xA2: REG1 = &X; break;
    case 0xA9: REG1 = &A; break;

    /* Load Zero Page */
    case 0xA5: REG1 = &A; break;
    case 0xA6: REG1 = &X; break;
    case 0xA4: REG1 = &Y; break;

    /* Load Absolute */
    case 0xAC: REG1 = &A; break;
    case 0xAD: REG1 = &Y; break;
    case 0xAE: REG1 = &X; break;

    /* Load Zero Page Indexed */
    case 0xB4: REG1 = &Y; REG2 = &X; break;
    case 0xB5: REG1 = &A; REG2 = &X; break;
    case 0xB6: REG1 = &X; REG2 = &Y; break;
  }

  return params;
}

INSTRUCTION(ld_imm)
{
  assert_or_fatal(REG1 != NULL);
  *REG1 = memory_get_next_byte(params->memory, params->cpu);
  set_zero_and_negative_flags(params->cpu, *REG1);
}

INSTRUCTION(ld_zpg)
{
  assert_or_fatal(REG1 != NULL);
  uint8_t address = memory_get_next_byte(params->memory, params->cpu);
  *REG1 = memory_get_byte(params->memory, address);
  set_zero_and_negative_flags(params->cpu, *REG1);
}

INSTRUCTION(ld_zpg_idx)
{
  assert_or_fatal(REG1 != NULL && REG2 != NULL);
  uint8_t address = memory_get_next_byte(params->memory, params->cpu);
  *REG1 = memory_get_byte(params->memory, get_zpg_index_address(address, *REG2));
  set_zero_and_negative_flags(params->cpu, *REG1);
}

INSTRUCTION(ld_abs)
{
  assert_or_fatal(REG1 != NULL);
  uint8_t address = get_abs_address(params->memory, params->cpu);
  *REG1 = memory_get_byte(params->memory, address);
  set_zero_and_negative_flags(params->cpu, *REG1);
}

INSTRUCTION(st_zpg)
{
  assert_or_fatal(REG1 != NULL);
  uint8_t address = memory_get_next_byte(params->memory, params->cpu);
  memory_set_byte(params->memory, address, *REG1);
}

INSTRUCTION(st_abs)
{
  assert_or_fatal(REG1 != NULL);
  uint8_t address = get_abs_address(params->memory, params->cpu);
  memory_set_byte(params->memory, address, *REG1);
}
