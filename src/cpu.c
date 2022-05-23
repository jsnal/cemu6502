#include "cpu.h"
#include "log.h"
#include <stdlib.h>
#include <stdio.h>

cpu_t *cpu_create()
{
  cpu_t *cpu = (cpu_t*) malloc(sizeof(cpu_t));
  cpu->a = cpu->x = cpu->y = cpu->pc = cpu->ps = cpu->sp = 0;

  return cpu;
}

bool get_processor_status(const cpu_t *cpu, enum ps_flag flag)
{
  return (cpu->ps >> flag) & 1;
}

void set_processor_status(cpu_t *cpu, enum ps_flag flag, bool val)
{
  if (val) {
    cpu->ps |= 1 << flag;
  } else {
    cpu->ps &= ~(1 << flag);
  }
}

// TODO: This should return a string not print
void cpu_registers_to_string(const cpu_t *cpu)
{
  dbgln("PC: 0x%04X SP: 0x%02X A: 0x%02X X: 0x%02X Y: 0x%02X", cpu->pc, cpu->sp, cpu->a, cpu->x, cpu->y);
  dbgln("N: %d V: %d B: %d D: %d I: %d Z: %d C: %d",
        get_processor_status(cpu, PS_NEGATIVE),
        get_processor_status(cpu, PS_OVERFLOW),
        get_processor_status(cpu, PS_BREAK),
        get_processor_status(cpu, PS_DECIMAL),
        get_processor_status(cpu, PS_INTERRUPT),
        get_processor_status(cpu, PS_ZERO),
        get_processor_status(cpu, PS_CARRY));
}
