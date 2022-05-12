#ifndef HANDLERS_H
#define HANDLERS_H

#include "cpu.h"
#include "memory.h"
#include "machine.h"

int (*handler_get(int (**handlers)(handler_params_t*), uint8_t opcode))(handler_params_t*);
handler_params_t *handler_get_params(struct machine_struct*, uint8_t opcode);
int handler_ld_imm(handler_params_t*);
int handler_ld_zpg(handler_params_t*);

#endif
