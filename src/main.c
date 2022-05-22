#include "machine.h"
#include "log.h"
#include <stdlib.h>
#include <stdio.h>


int main()
{
  set_quiet_mode(false);
  machine_t *machine = machine_create();

//  uint8_t program[] = {
//    0xA9, 0x01,
//    0xA2, 0x15,
//    0xA0, 0xFF,
//    0xA5, 0x0F,
//    0xA6, 0x0F,
//    0x84, 0x10
//  };

  /* For testing Load Absolute */
/*  uint8_t program[] = { 0xA2, 0x05, 0xEA, 0x8E, 0x7F, 0x11, 0xAC, 0x7F, 0x11 }; */


  /* For testing Load Zero-Page Index */
//  uint8_t program[] = { 0xA2, 0x05, 0x86, 0x7F, 0xA2, 0xFF, 0xB4, 0x80 };

  /* For testing Load Indirect Index */
//  uint8_t program[] = { 0XA2, 0X05, 0XA0, 0X88, 0X84, 0X0A, 0XA0, 0X1B, 0X84, 0X0B, 0XA0, 0X15, 0X8C, 0X88, 0X1B, 0XA1, 0X05 };

  /* For testing Load Index Indirect */
  uint8_t program[] = { 0XA0, 0X05, 0XA2, 0X88, 0X86, 0X0A, 0XA2, 0X1B, 0X86, 0X0B, 0XA2, 0X15, 0X8E, 0X8D, 0X1B, 0XB1, 0X0A };

  machine_load(machine, program, sizeof(program) / sizeof(uint8_t), 0x0600);

  // Set a byte on the zero-page 0x00-0xFF
  memory_set_byte(machine->memory, 0x0F, 0xAA);

  machine_execute(machine);
}
