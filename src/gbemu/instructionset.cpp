#include "cpu.h"


void CPU::z80_nop()
{

}


void CPU::z80_ld_reg16_dat16(uint16_t *reg16)
{
    *reg16 = memory->readByte(registers.PC);
    registers.PC++;
    *reg16 += memory->readByte(registers.PC) << 8;
    registers.PC++;
}


void CPU::z80_ld_reg16_addr_reg8(uint16_t *reg16, uint8_t *reg8) {
    memory->writeByte(*reg16, *reg8);
    registers.PC++;
}


void CPU::z80_inc_reg16(uint16_t *reg16)
{
    *reg16 += 1;
}


void CPU::z80_inc_reg8(uint8_t *reg8)
{
    *reg8 += 1;
}


void CPU::z80_dec_reg8(uint8_t *reg8)
{
    *reg8 -= 1;
}


void CPU::z80_ld_reg8_dat8(uint8_t *reg8)
{
    *reg8 = memory->readByte(registers.PC);
    registers.PC++;
}


void CPU::z80_rlca()
{

}


void CPU::z80_ld_addr_sp()
{

}


void CPU::z80_add_reghl_reg16(uint16_t *reg16)
{
    registers.HL += *reg16;
}


void CPU::z80_ld_rega_addr_reg16(uint16_t *reg16)
{
    registers.A = memory->readByte(*reg16);
}


void CPU::z80_dec_reg16(uint16_t *reg16)
{
    *reg16 -= 1;
}


void CPU::z80_rrca()
{

}


void CPU::z80_stop()
{

}


void CPU::z80_rla()
{

}


void CPU::z80_jr_offset()
{
    registers.PC += (int8_t)memory->readByte(registers.PC);
}


void CPU::z80_rra()
{

}


void CPU::z80_jr_nz()
{

}


void CPU::z80_ldi_reghl_addr_rega()
{
    memory->writeByte(registers.HL, registers.A);
    registers.HL++;
}


void CPU::z80_daa()
{

}


void CPU::z80_jr_z()
{

}


void CPU::z80_ldi_rega_reghl_addr()
{
    registers.A = memory->readByte(registers.HL);
    registers.HL++;
}


void CPU::z80_cpl()
{

}


void CPU::z80_jr_nc()
{

}


void CPU::z80_ldd_reghl_addr_rega()
{
    memory->writeByte(registers.HL, registers.A);
    registers.HL++;
}


void CPU::z80_inc_reghl_addr()
{
    memory->writeByte(registers.HL, memory->readByte(registers.HL) + 1);
}


void CPU::z80_dec_reghl_addr()
{
    memory->writeByte(registers.HL, memory->readByte(registers.HL) - 1);
}


void CPU::z80_ld_reghl_addr_dat8()
{
    memory->writeByte(registers.HL, memory->readByte(registers.PC));
    registers.PC++;
}


void CPU::z80_scf()
{

}


void CPU::z80_ldd_rega_reghl_addr()
{
    registers.A = memory->readByte(registers.HL);
    registers.HL++;
}


void CPU::z80_jr_c()
{

}


void CPU::z80_ccf()
{

}


void CPU::z80_ld_reg8_reg8(uint8_t *reg8dest, uint8_t *reg8src)
{
    *reg8dest = *reg8src;
}


void CPU::z80_ld_reg8_reghl_addr(uint8_t *reg8)
{
    *reg8 = memory->readByte(registers.HL);
}


void CPU::z80_ld_reghl_addr_reg8(uint8_t *reg8)
{
    memory->writeByte(registers.HL, *reg8);
}


void CPU::z80_halt()
{

}


void CPU::z80_add_rega_reg8(uint8_t *reg8)
{

}


void CPU::z80_add_rega_reghl_addr()
{

}


void CPU::z80_adc_rega_reg8(uint8_t *reg8)
{

}


void CPU::z80_adc_rega_reghl_addr()
{

}


void CPU::z80_sub_rega_reg8(uint8_t *reg8)
{

}


void CPU::z80_sub_rega_reghl_addr()
{

}


void CPU::z80_sbc_rega_reg8(uint8_t *reg8)
{

}


void CPU::z80_sbc_rega_reghl_addr()
{

}


void CPU::z80_jp_a16() {
    registers.PC = memory->readByte(registers.PC) + (memory->readByte(registers.PC + 1) << 8);
}
