#include "machine.h"
#include "utils.h"
#include "log.h"

#define START 0x0600
#define CPU machine->cpu
#define MEMORY machine->memory

static machine_t *machine;

static void test_adc()
{
  machine_reset(machine);
  uint8_t program1[] = { 0xA9, 0x80, 0x85, 0x01, 0x69, 0x01 };
  machine_load(machine, program1, sizeof(program1) / sizeof(uint8_t), START);
  machine_execute(machine);

  assert_or_fatal(CPU->a == 0x81);
  assert_or_fatal(get_processor_status(CPU, PS_NEGATIVE));

  machine_reset(machine);
  uint8_t program2[] = { 0xA9, 0x80, 0x85, 0x01, 0x69, 0x80 };
  machine_load(machine, program2, sizeof(program2) / sizeof(uint8_t), START);
  machine_execute(machine);

  assert_or_fatal(CPU->a == 0x00);
  assert_or_fatal(!get_processor_status(CPU, PS_NEGATIVE));
  assert_or_fatal(get_processor_status(CPU, PS_OVERFLOW));
  assert_or_fatal(get_processor_status(CPU, PS_ZERO));
  assert_or_fatal(get_processor_status(CPU, PS_CARRY));
}

int main()
{
  set_quiet_mode(true);
  machine = machine_create();

  test_adc();
}
