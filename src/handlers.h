#ifndef HANDLERS_H
#define HANDLERS_H

#include "cpu.h"
#include "memory.h"
#include "machine.h"

#define INSTRUCTION(name) void handler_##name(handler_params_t *params)
#define REG1 params->reg1
#define REG2 params->reg2
#define fail handler_fail

typedef struct handler_params_struct {
  machine_t *machine;
  uint8_t *reg1;
  uint8_t *reg2;
} handler_params_t;

void (*handler_get(uint8_t opcode))(handler_params_t*);
handler_params_t *handler_get_params(handler_params_t*, machine_t*, uint8_t);

/* Prototypes for instruction handlers */
void handler_fail(handler_params_t*);
void handler_nop(handler_params_t*);
void handler_ld_imm(handler_params_t*);
void handler_ld_zpg(handler_params_t*);
void handler_ld_zpg_idx(handler_params_t*);
void handler_ld_abs(handler_params_t*);
void handler_st_zpg(handler_params_t*);
void handler_st_abs(handler_params_t*);

#endif
