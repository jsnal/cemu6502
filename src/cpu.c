#include "cpu.h"
#include <stdlib.h>

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
