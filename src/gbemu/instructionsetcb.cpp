#include "Cpu.h"
#include "MemoryMap.h"

void CPU::z80_cb_reset_reg8_bit(uint8_t *reg8, uint8_t bitMask)
{
    *reg8 &= bitMask;
}


void CPU::z80_cb_reset_reghl_addr16_bit(uint8_t bitMask)
{
    memoryMap->writeByte(registers.HL, memoryMap->readByte(registers.HL) & bitMask);
}


void CPU::z80_cb_rl_reg8(uint8_t *reg8)
{
    bool carry = registers.flagC;

    registers.flagC = *reg8 & 0x80;
    *reg8 = (*reg8 << 1) | (uint8_t)carry;

    if (*reg8 == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;
    registers.flagH = false;
}


void CPU::z80_cb_rl_reghl_addr16()
{
    bool carry = registers.flagC;
    uint8_t data = memoryMap->readByte(registers.HL);

    registers.flagC = data & 0x80;
    data = (data << 1) | (uint8_t)carry;
    memoryMap->writeByte(registers.HL, data);

    if (data == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;
    registers.flagH = false;
}


void CPU::z80_cb_rlc_reg8(uint8_t *reg8)
{
    registers.flagC = *reg8 & 0x80;
    *reg8 = (*reg8 << 1) | ((*reg8 & 0x80) >> 7);

    if (*reg8 == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;
    registers.flagH = false;
}


void CPU::z80_cb_rlc_reghl_addr16()
{
    uint8_t data = memoryMap->readByte(registers.HL);
    registers.flagC = data & 0x80;

    data = (data << 1) | ((data & 0x80) >> 7);
    memoryMap->writeByte(registers.HL, data);

    if (data == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;
    registers.flagH = false;
}


void CPU::z80_cb_rr_reg8(uint8_t *reg8)
{
    bool carry = registers.flagC;
    registers.flagC = *reg8 & 0x01;
    *reg8 = (*reg8 >> 1) | (uint8_t)(carry << 7);

    if (*reg8 == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;
    registers.flagH = false;
}


void CPU::z80_cb_rr_reghl_addr16()
{
    bool carry = registers.flagC;
    uint8_t data = memoryMap->readByte(registers.HL);

    registers.flagC = data & 0x01;
    data = (data >> 1) | (uint8_t)(carry << 7);
    memoryMap->writeByte(registers.HL, data);

    if (data == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;
    registers.flagH = false;
}


void CPU::z80_cb_rrc_reg8(uint8_t *reg8)
{
    registers.flagC = *reg8 & 0x01;
    *reg8 = (*reg8 >> 1) | ((*reg8 & 0x01) << 7);

    if (*reg8 == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;
    registers.flagH = false;
}


void CPU::z80_cb_rrc_reghl_addr16()
{
    uint8_t data = memoryMap->readByte(registers.HL);

    registers.flagC = data & 0x01;
    data = (data >> 1) | ((data & 0x01) << 7);
    memoryMap->writeByte(registers.HL, data);

    if (data == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;
    registers.flagH = false;
}


void CPU::z80_cb_set_reg8_bit(uint8_t *reg8, uint8_t bitMask)
{
    *reg8 |= bitMask;
}


void CPU::z80_cb_set_reghl_addr16_bit(uint8_t bitMask) {
    uint8_t data = memoryMap->readByte(registers.HL);

    data |= bitMask;
    memoryMap->writeByte(registers.HL, data);
}


void CPU::z80_cb_sla_reg8(uint8_t *reg8)
{
    if (*reg8 & 0x80)
        registers.flagC = true;
    else
        registers.flagC = false;

    *reg8 <<= 1;

    if (*reg8 == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;
    registers.flagH = false;
}


void CPU::z80_cb_sla_reghl_addr16()
{
    uint8_t data = memoryMap->readByte(registers.HL);

    if (data & 0x80)
        registers.flagC = true;
    else
        registers.flagC = false;;

    data <<= 1;
    memoryMap->writeByte(registers.HL, data);

    if (data == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;
    registers.flagH = false;
}


void CPU::z80_cb_sra_reg8(uint8_t *reg8)
{
    if (*reg8 & 0x01)
        registers.flagC = true;
    else
        registers.flagC = false;

    // Preserve the sign bit.
    *reg8 = (*reg8 >> 1) | (*reg8 & 0x80);

    if (*reg8 == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;
    registers.flagH = false;
}


void CPU::z80_cb_sra_reghl_addr16()
{
    uint8_t data = memoryMap->readByte(registers.HL);

    if (data & 0x01)
        registers.flagC = true;
    else
        registers.flagC = false;

    // Preserve the sign bit.
    data = (data >> 1) | (data & 0x80);
    memoryMap->writeByte(registers.HL, data);

    if (data == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;
    registers.flagH = false;
}


void CPU::z80_cb_srl_reg8(uint8_t *reg8)
{
    if (*reg8 & 0x01)
        registers.flagC = true;
    else
        registers.flagC = false;

    *reg8 >>= 1;

    if (*reg8 == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;
    registers.flagH = false;
}


void CPU::z80_cb_srl_reghl_addr16()
{
    uint8_t data = memoryMap->readByte(registers.HL);

    if (data & 0x01)
        registers.flagC = true;
    else
        registers.flagC = false;

    data >>= 1;
    memoryMap->writeByte(registers.HL, data);

    if (data == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;
    registers.flagH = false;
}


void CPU::z80_cb_swap_reg8(uint8_t *reg8)
{
    *reg8 = (*reg8 << 4) | (*reg8 >> 4);

    if (*reg8 == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;
    registers.flagH = false;
    registers.flagC = false;
}


void CPU::z80_cb_swap_reghl_addr16()
{
    uint8_t data = memoryMap->readByte(registers.HL);

    data = (data << 4) | (data >> 4);
    memoryMap->writeByte(registers.HL, data);

    if (data == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;
    registers.flagH = false;
    registers.flagC = false;
}


void CPU::z80_cb_test_reg8_bit(uint8_t *reg8, uint8_t bitMask)
{
    if (*reg8 & bitMask)
        registers.flagZ = false;
    else
        registers.flagZ = true;

    registers.flagN = false;
    registers.flagH = true;
}


void CPU::z80_cb_test_reghl_addr16_bit(uint8_t bitMask)
{
    uint8_t data = memoryMap->readByte(registers.HL);

    if (data & bitMask)
        registers.flagZ = false;
    else
        registers.flagZ = true;

    registers.flagN = false;
    registers.flagH = true;
}
