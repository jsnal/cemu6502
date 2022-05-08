#include "cpu.h"
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
  printf("PC: 0x%04X PS: 0x%02X SP: 0x%02X A: 0x%02X X: 0x%02X Y: 0x%02X\n", cpu->pc, cpu->ps, cpu->sp, cpu->a, cpu->x, cpu->y);
}
