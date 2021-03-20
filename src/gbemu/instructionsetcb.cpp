#include "cpu.h"


void CPU::z80_cb_rl_reg8(uint8_t *reg8)
{
    *reg8 = ((*reg8 & 0x80) >> 7) | (*reg8 << 1);

    if (*reg8 == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;
    registers.flagH = true;
}


void CPU::z80_cb_test_reg8_bit7(uint8_t *reg8)
{
    if (*reg8 & 0x80)
        registers.flagZ = false;
    else
        registers.flagZ = true;

    registers.flagN = false;
    registers.flagH = true;
}
