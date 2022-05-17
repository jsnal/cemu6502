#ifndef HANDLERS_H
#define HANDLERS_H

#include "cpu.h"
#include "memory.h"
#include "machine.h"

#define REG1 params->reg1
#define REG2 params->reg2
#define fail handler_fail
#define INSTRUCTION(name) void handler_##name(handler_params_t *params)

#define PARAM1(opcode, r1) case opcode: REG1 = &r1; break
#define PARAM2(opcode, r1, r2) case opcode: REG1 = &r1; REG2 = &r2; break
#define GET_ARGS(arg1, arg2, arg3, arg4, ...) arg4
#define PARAM_CHOOSE(...) GET_ARGS(__VA_ARGS__, PARAM2, PARAM1,)
#define PARAM(...) PARAM_CHOOSE(__VA_ARGS__)(__VA_ARGS__)

typedef struct handler_params_struct {
  machine_t *machine;
  uint8_t *reg1;
  uint8_t *reg2;
} handler_params_t;

void (*handler_get(uint8_t opcode))(handler_params_t*);
handler_params_t *handler_get_params(handler_params_t*, machine_t*, uint8_t);

/* Prototypes for instruction handlers */
INSTRUCTION(fail);
INSTRUCTION(nop);
INSTRUCTION(ld_imm);
INSTRUCTION(ld_zpg);
INSTRUCTION(ld_zpg_idx);
INSTRUCTION(ld_abs);
INSTRUCTION(ld_abs_idx);
INSTRUCTION(st_zpg);
INSTRUCTION(st_abs);

#endif
