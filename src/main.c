#include "machine.h"
#include <stdlib.h>
#include <stdio.h>

#include "log.h"

int main()
{
  machine_t *machine = machine_create();

  uint8_t program[] = { 0xA9, 0x01, 0xA2, 0x15, 0xA0, 0xFF };

  machine_load(machine, program, sizeof(program) / sizeof(uint8_t), 0x0600);

  machine_execute(machine);

  set_quiet_mode(false);
  dbgln("%s", "Hello World!");
  wrnln("%s", "Hello World!");
  infln("%s", "Hello World!");
  errln("%s", "Hello World!");
}
