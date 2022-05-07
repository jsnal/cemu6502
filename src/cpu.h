#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include <stdint.h>

enum ps_flag {
  PS_CARRY = 1,
  PS_ZERO = 2,
  PS_INTERRUPT = 3,
  PS_DECIMAL = 4,
  PS_BREAK = 5,
  PS_OVERFLOW = 6,
  PS_NEGATIVE = 7
};

typedef struct cpu_struct {
  uint16_t pc;
  uint8_t ps, sp, a, x, y;
} cpu_t;

cpu_t *cpu_create();
bool get_processor_status(const cpu_t*, enum ps_flag);
void set_processor_status(cpu_t*, enum ps_flag, bool);

#endif
