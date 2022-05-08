#ifndef HANDLERS_H
#define HANDLERS_H

#include "machine.h"

int (*handler_get(int (**handlers)(machine_t*), uint8_t opcode))(machine_t*);
int handler_lda_imm(machine_t*);
int handler_ldx_imm(machine_t*);
int handler_ldy_imm(machine_t*);

#endif
