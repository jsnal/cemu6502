#include "handlers.h"
#include "machine.h"
#include "memory.h"
#include "utils.h"
#include <stddef.h>

static void (*handlers[256])(handler_params_t*) = {
/* 0x00            0x01  0x02            0x03  0x04                0x05                0x06                0x07  0x08  0x09                0x0A         0x0B  0x0C                0x0D                0x0E                0x0F*/
   fail,           fail, fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0x00 */
   fail,           fail, fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0x10 */
   fail,           fail, fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0x20 */
   fail,           fail, fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0x30 */
   fail,           fail, fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0x40 */
   fail,           fail, fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0x50 */
   fail,           fail, fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0x60 */
   fail,           fail, fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0x70 */
   fail,           fail, fail,           fail, handler_st_zpg,     handler_st_zpg,     handler_st_zpg,     fail, fail, fail,               fail,        fail, handler_st_abs,     handler_st_abs,     handler_st_abs,     fail, /* 0x80 */
   fail,           fail, fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0x90 */
   handler_ld_imm, fail, handler_ld_imm, fail, handler_ld_zpg,     handler_ld_zpg,     handler_ld_zpg,     fail, fail, handler_ld_imm,     fail,        fail, handler_ld_abs,     handler_ld_abs,     handler_ld_abs,     fail, /* 0xA0 */
   fail,           fail, fail,           fail, handler_ld_zpg_idx, handler_ld_zpg_idx, handler_ld_zpg_idx, fail, fail, handler_ld_abs_idx, fail,        fail, handler_ld_abs_idx, handler_ld_abs_idx, handler_ld_abs_idx, fail, /* 0xB0 */
   fail,           fail, fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0xC0 */
   fail,           fail, fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0xD0 */
   fail,           fail, fail,           fail, fail,               fail,               fail,               fail, fail, fail,               handler_nop, fail, fail,               fail,               fail,               fail, /* 0xE0 */
   fail,           fail, fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0xF0 */
};

static void set_zero_and_negative_flags(cpu_t *cpu, uint8_t value)
{
  set_processor_status(cpu, PS_ZERO, value == 0);
  set_processor_status(cpu, PS_NEGATIVE, (value & 0x80) == 0x80);
}

static uint8_t get_zpg_index_address(uint8_t address, uint8_t reg)
{
  return address + reg;
}

static uint16_t get_abs_address(machine_t *machine)
{
  uint8_t low = memory_get_next_byte(machine->memory, machine->cpu);
  uint8_t high = memory_get_next_byte(machine->memory, machine->cpu);
  return (high << 8) + low;
}

static uint16_t get_abs_index_address(machine_t *machine, uint8_t reg)
{
  return get_abs_address(machine) + reg;
}

void (*handler_get(uint8_t opcode))(handler_params_t*)
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

    /* Load Absolute Indexed */
    case 0xB9: REG1 = &A; REG2 = &Y; break;
    case 0xBC: REG1 = &Y; REG2 = &X; break;
    case 0xBD: REG1 = &A; REG2 = &X; break;
    case 0xBE: REG1 = &X; REG2 = &Y; break;
  }

  return params;
}

INSTRUCTION(fail)
{
  errln("Invalid instruction found...");
  fatal();
}

INSTRUCTION(nop) { }

INSTRUCTION(ld_imm)
{
  assert_or_fatal(REG1);
  *REG1 = memory_get_next_byte(params->machine->memory, params->machine->cpu);
  set_zero_and_negative_flags(params->machine->cpu, *REG1);
}

INSTRUCTION(ld_zpg)
{
  assert_or_fatal(REG1);
  uint8_t address = memory_get_next_byte(params->machine->memory, params->machine->cpu);
  *REG1 = memory_get_byte(params->machine->memory, address);
  set_zero_and_negative_flags(params->machine->cpu, *REG1);
}

INSTRUCTION(ld_zpg_idx)
{
  assert_or_fatal(REG1 && REG2);
  uint8_t address = memory_get_next_byte(params->machine->memory, params->machine->cpu);
  *REG1 = memory_get_byte(params->machine->memory, get_zpg_index_address(address, *REG2));
  set_zero_and_negative_flags(params->machine->cpu, *REG1);
}

INSTRUCTION(ld_abs)
{
  assert_or_fatal(REG1);
  uint16_t address = get_abs_address(params->machine);
  *REG1 = memory_get_byte(params->machine->memory, address);
  set_zero_and_negative_flags(params->machine->cpu, *REG1);
}

INSTRUCTION(ld_abs_idx)
{
  assert_or_fatal(REG1 && REG2);
  uint16_t address = get_abs_index_address(params->machine, *REG2);
  *REG1 = memory_get_byte(params->machine->memory, address);
  set_zero_and_negative_flags(params->machine->cpu, *REG1);
}

INSTRUCTION(st_zpg)
{
  assert_or_fatal(REG1);
  uint8_t address = memory_get_next_byte(params->machine->memory, params->machine->cpu);
  memory_set_byte(params->machine->memory, address, *REG1);
}

INSTRUCTION(st_abs)
{
  assert_or_fatal(REG1);
  uint8_t address = get_abs_address(params->machine);
  memory_set_byte(params->machine->memory, address, *REG1);
}
