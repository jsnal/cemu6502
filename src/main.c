#include "machine.h"
#include "log.h"
#include <stdlib.h>
#include <stdio.h>

static uint8_t *read_binary(const char *filename, int *file_length)
{
  FILE *fp = fopen(filename, "rb");
  fseek(fp, 0, SEEK_END);
  long length = ftell(fp);
  rewind(fp);

  uint8_t *buffer = (uint8_t*) malloc(sizeof(uint8_t) * length);
  fread(buffer, length, 1, fp);
  fclose(fp);

  infln("Using binary: %s (%ld bytes)", filename, length);

  *file_length = length;
  return buffer;
}

int main()
{
  set_quiet_mode(false);
  machine_t *machine = machine_create();

  int length = 0;
  uint8_t *program = read_binary("tests/adc_imm.bin", &length);

  machine_load(machine, program, length, 0x0600);

  // Set a byte on the zero-page 0x00-0xFF
  memory_set_byte(machine->memory, 0x0F, 0xAA);

  machine_execute(machine);
}
