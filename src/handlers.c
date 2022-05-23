#include "handlers.h"
#include "machine.h"
#include "memory.h"
#include "utils.h"
#include <stddef.h>

static void (*handlers[256])(handler_params_t*) = {
/* 0x00            0x01                0x02            0x03  0x04                0x05                0x06                0x07  0x08  0x09                0x0A         0x0B  0x0C                0x0D                0x0E                0x0F*/
   fail,           fail,               fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0x00 */
   fail,           fail,               fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0x10 */
   fail,           fail,               fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0x20 */
   fail,           fail,               fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0x30 */
   fail,           fail,               fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0x40 */
   fail,           fail,               fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0x50 */
   fail,           fail,               fail,           fail, fail,               fail,               fail,               fail, fail, handler_adc_imm,    fail,        fail, fail,               fail,               fail,               fail, /* 0x60 */
   fail,           fail,               fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0x70 */
   fail,           fail,               fail,           fail, handler_st_zpg,     handler_st_zpg,     handler_st_zpg,     fail, fail, fail,               fail,        fail, handler_st_abs,     handler_st_abs,     handler_st_abs,     fail, /* 0x80 */
   fail,           fail,               fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0x90 */
   handler_ld_imm, handler_ld_ind_idx, handler_ld_imm, fail, handler_ld_zpg,     handler_ld_zpg,     handler_ld_zpg,     fail, fail, handler_ld_imm,     fail,        fail, handler_ld_abs,     handler_ld_abs,     handler_ld_abs,     fail, /* 0xA0 */
   fail,           handler_ld_idx_ind, fail,           fail, handler_ld_zpg_idx, handler_ld_zpg_idx, handler_ld_zpg_idx, fail, fail, handler_ld_abs_idx, fail,        fail, handler_ld_abs_idx, handler_ld_abs_idx, handler_ld_abs_idx, fail, /* 0xB0 */
   fail,           fail,               fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0xC0 */
   fail,           fail,               fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0xD0 */
   fail,           fail,               fail,           fail, fail,               fail,               fail,               fail, fail, fail,               handler_nop, fail, fail,               fail,               fail,               fail, /* 0xE0 */
   fail,           fail,               fail,           fail, fail,               fail,               fail,               fail, fail, fail,               fail,        fail, fail,               fail,               fail,               fail, /* 0xF0 */
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

static uint16_t get_ind_index_address(machine_t *machine, uint8_t reg)
{
  uint8_t address = memory_get_next_byte(machine->memory, machine->cpu) + reg;
  uint8_t low = memory_get_byte(machine->memory, address);
  uint8_t high = memory_get_byte(machine->memory, address + 1);
  return (high << 8) + low;
}

static uint16_t get_index_ind_address(machine_t *machine, uint8_t reg)
{
  uint8_t address = memory_get_next_byte(machine->memory, machine->cpu);
  uint8_t low = memory_get_byte(machine->memory, address);
  uint8_t high = memory_get_byte(machine->memory, address + 1);
  return (high << 8) + low + reg;
}

static void add_with_carry(machine_t *machine, uint8_t value)
{
  uint8_t old_a = machine->cpu->a;
  uint16_t sum = value + old_a;

  if (get_processor_status(machine->cpu, PS_CARRY)) {
    sum += 1;
  }

  machine->cpu->a = (uint8_t) sum;
  uint8_t new_a = machine->cpu->a;
  set_zero_and_negative_flags(machine->cpu, new_a);
  set_processor_status(machine->cpu, PS_CARRY, sum > 0xFF);
  set_processor_status(machine->cpu, PS_OVERFLOW,
                       (value < 0x7F && old_a < 0x7F && new_a > 0x7F) ||
                       (value > 0x7F && old_a > 0x7F && new_a < 0x7F));
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
    PARAM(0x84, Y);
    PARAM(0x85, A);
    PARAM(0x86, X);

    PARAM(0x8C, Y);
    PARAM(0x8D, A);
    PARAM(0x8E, X);

    /* Load Immediate */
    PARAM(0xA0, Y);
    PARAM(0xA2, X);
    PARAM(0xA9, A);

    /* Load Zero Page */
    PARAM(0xA5, A);
    PARAM(0xA6, X);
    PARAM(0xA4, Y);

    /* Load Absolute */
    PARAM(0xAC, A);
    PARAM(0xAD, Y);
    PARAM(0xAE, X);

    /* Load Zero Page Indexed */
    PARAM(0xB4, Y, X);
    PARAM(0xB5, A, X);
    PARAM(0xB6, X, Y);

    /* Load Absolute Indexed */
    PARAM(0xB9, A, Y);
    PARAM(0xBC, Y, X);
    PARAM(0xBD, A, X);
    PARAM(0xBE, X, Y);

    /* Load Indirect */
    PARAM(0xA1, A, X);
    PARAM(0xB1, A, Y);
  }

  return params;
}

INSTRUCTION(adc_imm)
{
  add_with_carry(params->machine, memory_get_next_byte(params->machine->memory, params->machine->cpu));
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

INSTRUCTION(ld_ind_idx)
{
  assert_or_fatal(REG1 && REG2);
  uint16_t address = get_ind_index_address(params->machine, *REG2);
  *REG1 = memory_get_byte(params->machine->memory, address);
  set_zero_and_negative_flags(params->machine->cpu, *REG1);
}

INSTRUCTION(ld_idx_ind)
{
  assert_or_fatal(REG1 && REG2);
  uint16_t address = get_index_ind_address(params->machine, *REG2);
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
  uint16_t address = get_abs_address(params->machine);
  memory_set_byte(params->machine->memory, address, *REG1);
}
