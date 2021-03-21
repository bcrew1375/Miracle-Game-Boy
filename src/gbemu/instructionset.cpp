#include "cpu.h"


void CPU::z80_adc_rega_dat8()
{
    int16_t result = registers.A + memory->readByte(registers.PC) + registers.flagC;
    registers.PC++;

    registers.A = (uint8_t)result;

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = 0;

    if (result > 0xFF)
        registers.flagC = true;
    else
        registers.flagC = false;
}


void CPU::z80_adc_rega_reg8(uint8_t *reg8)
{
    int16_t result = registers.A + *reg8 + registers.flagC;
    registers.A = result;

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    if (result > 0xFF)
        registers.flagC = true;
    else
        registers.flagC = false;
}


void CPU::z80_adc_rega_reghl_addr16()
{
    int16_t result = registers.A + memory->readByte(registers.HL) + registers.flagC;
    registers.A = result;

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    if (result > 0xFF)
        registers.flagC = true;
    else
        registers.flagC = false;
}


void CPU::z80_add_rega_dat8()
{
    int16_t result = registers.A + memory->readByte(registers.PC);
    registers.PC++;
    registers.A = result;

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    if (result > 0xFF)
        registers.flagC = true;
    else
        registers.flagC = false;
}


void CPU::z80_add_rega_reg8(uint8_t *reg8)
{
    int16_t result = registers.A + *reg8;
    registers.A = result;

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    if (result > 0xFF)
        registers.flagC = true;
    else
        registers.flagC = false;
}


void CPU::z80_add_rega_reghl_addr16()
{
    int16_t result = registers.A + memory->readByte(registers.HL);
    registers.A = result;

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    if (result > 0xFF)
        registers.flagC = true;
    else
        registers.flagC = false;
}


void CPU::z80_add_reghl_reg16(uint16_t *reg16)
{
    registers.HL += *reg16;
}


void CPU::z80_add_sp_dat8()
{
    registers.SP += (int8_t)memory->readByte(registers.PC);
    registers.PC++;
}


void CPU::z80_call()
{
    registers.PC += 2;
    z80_push_reg16(&registers.PC);

    registers.PC = memory->readByte(registers.PC - 2) + (memory->readByte(registers.PC - 1) << 8);
}


void CPU::z80_call_c()
{
    registers.PC += 2;

    if (registers.flagC == true) {
        z80_push_reg16(&registers.PC);

        registers.PC = memory->readByte(registers.PC - 2) + (memory->readByte(registers.PC - 1) << 8);

        clockCyclesExecuted += 12;
    }
}


void CPU::z80_call_nc()
{
    registers.PC += 2;

    if (registers.flagC == false) {
        z80_push_reg16(&registers.PC);

        registers.PC = memory->readByte(registers.PC - 2) + (memory->readByte(registers.PC - 1) << 8);

        clockCyclesExecuted += 12;
    }
}


void CPU::z80_call_nz()
{
    registers.PC += 2;

    if (registers.flagZ == false) {
        z80_push_reg16(&registers.PC);

        registers.PC = memory->readByte(registers.PC - 2) + (memory->readByte(registers.PC - 1) << 8);

        clockCyclesExecuted += 12;
    }
}


void CPU::z80_call_z()
{
    registers.PC += 2;

    if (registers.flagZ == true) {
        z80_push_reg16(&registers.PC);

        registers.PC = memory->readByte(registers.PC - 2) + (memory->readByte(registers.PC - 1) << 8);

        clockCyclesExecuted += 12;
    }
}


void CPU::z80_ccf()
{
    registers.flagN = false;
    registers.flagH = false;
    registers.flagC ^= 1;
}


void CPU::z80_cpl()
{
    registers.A ^= 0xFF;

    registers.flagN = true;
    registers.flagH = true;
}


void CPU::z80_daa()
{

}


void CPU::z80_dec_reg16(uint16_t *reg16)
{
    *reg16 -= 1;
}


void CPU::z80_dec_reg8(uint8_t *reg8)
{
    *reg8 -= 1;

    if (*reg8 == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;

    if (*reg8 == 0x0F)
        registers.flagH = true;
    else
        registers.flagH = false;
}


void CPU::z80_dec_reghl_addr16()
{
    uint8_t data = memory->readByte(registers.HL) - 1;
    memory->writeByte(registers.HL, data);

    if (data == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;

    if (data == 0x0F)
        registers.flagH = true;
    else
        registers.flagH = false;
}


void CPU::z80_di()
{
    interruptMasterEnableFlag = false;
}


void CPU::z80_ei()
{
    interruptMasterEnableFlag = true;
}


void CPU::z80_halt()
{
    halted = true;
}


void CPU::z80_inc_reg16(uint16_t *reg16)
{
    *reg16 += 1;
}


void CPU::z80_inc_reg8(uint8_t *reg8)
{
    *reg8 += 1;

    if (*reg8 == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;

    if (*reg8 == 0x10)
        registers.flagH = true;
    else
        registers.flagH = false;
}


void CPU::z80_inc_reghl_addr16()
{
    uint8_t data = memory->readByte(registers.HL) + 1;
    memory->writeByte(registers.HL, data);

    if (data == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    registers.flagN = false;

    if (data == 0x10)
        registers.flagH = true;
    else
        registers.flagH = false;
}


void CPU::z80_jp_a16() {
    registers.PC = memory->readByte(registers.PC) + (memory->readByte(registers.PC + 1) << 8);
}


void CPU::z80_jp_c()
{
    registers.PC += 2;
    if (registers.flagC == true) {
        registers.PC = memory->readByte(registers.PC - 2) + (memory->readByte(registers.PC - 1) << 8);
        clockCyclesExecuted += 4;
    }
}


void CPU::z80_jp_nc()
{
    registers.PC += 2;
    if (registers.flagC == false) {
        registers.PC = memory->readByte(registers.PC - 2) + (memory->readByte(registers.PC - 1) << 8);
        clockCyclesExecuted += 4;
    }
}


void CPU::z80_jp_nz()
{
    registers.PC += 2;
    if (registers.flagZ == false) {
        registers.PC = memory->readByte(registers.PC - 2) + (memory->readByte(registers.PC - 1) << 8);
        clockCyclesExecuted += 4;
    }
}


void CPU::z80_jp_reghl()
{
    //registers.PCl = memory->readByte(registers.HL);
    //registers.PCh = memory->readByte(registers.HL + 1);
    registers.PC = registers.HL;
}


void CPU::z80_jp_z()
{
    registers.PC += 2;
    if (registers.flagZ == true) {
        registers.PC = memory->readByte(registers.PC - 2) + (memory->readByte(registers.PC - 1) << 8);
        clockCyclesExecuted += 4;
    }
}


void CPU::z80_jr_c()
{
    int8_t offset = (int8_t)memory->readByte(registers.PC);
    registers.PC++;
    if (registers.flagC == true) {
        registers.PC += offset;
        clockCyclesExecuted += 4;
    }
}


void CPU::z80_jr_nc()
{
    int8_t offset = (int8_t)memory->readByte(registers.PC);
    registers.PC++;
    if (registers.flagC == false) {
        registers.PC += offset;
        clockCyclesExecuted += 4;
    }
}


void CPU::z80_jr_nz()
{
    int8_t offset = (int8_t)memory->readByte(registers.PC);
    registers.PC++;
    if (registers.flagZ == false) {
        registers.PC += offset;
        clockCyclesExecuted += 4;
    }
}


void CPU::z80_jr_offset()
{
    int8_t offset = (int8_t)memory->readByte(registers.PC);
    registers.PC++;
    registers.PC += offset;
}



void CPU::z80_jr_z()
{
    int8_t offset = (int8_t)memory->readByte(registers.PC);
    registers.PC++;
    if (registers.flagZ == true) {
        registers.PC += offset;
        clockCyclesExecuted += 4;
    }
}


void CPU::z80_ld_addr16_rega()
{
    uint16_t address = memory->readByte(registers.PC) + (memory->readByte(registers.PC + 1) << 8);
    registers.PC += 2;

    memory->writeByte(address, registers.A);
}


void CPU::z80_ld_addr16_sp()
{
    uint16_t address = memory->readByte(registers.PC) + (memory->readByte(registers.PC + 1) << 8);
    registers.PC += 2;

    memory->writeByte(address, registers.SPl);
    memory->writeByte(address + 1, registers.SPh);
}


void CPU::z80_ld_reg16_addr16_reg8(uint16_t *reg16, uint8_t *reg8) {
    memory->writeByte(*reg16, *reg8);
}


void CPU::z80_ld_reg16_dat16(uint16_t *reg16)
{
    *reg16 = memory->readByte(registers.PC);
    registers.PC++;
    *reg16 += memory->readByte(registers.PC) << 8;
    registers.PC++;
}


void CPU::z80_ld_reg8_dat8(uint8_t *reg8)
{
    *reg8 = memory->readByte(registers.PC);
    registers.PC++;
}


void CPU::z80_ld_reg8_reg8(uint8_t *reg8dest, uint8_t *reg8src)
{
    *reg8dest = *reg8src;
}


void CPU::z80_ld_reg8_reghl_addr16(uint8_t *reg8)
{
    *reg8 = memory->readByte(registers.HL);
}


void CPU::z80_ld_rega_addr16()
{
    registers.A = memory->readByte(memory->readByte(registers.PC) + (memory->readByte(registers.PC + 1) << 8));
    registers.PC += 2;
}


void CPU::z80_ld_rega_addr16_reg16(uint16_t *reg16)
{
    registers.A = memory->readByte(*reg16);
}


void CPU::z80_ld_rega_regc_port()
{
    registers.A = memory->readByte(0xFF00 + registers.C);
}


void CPU::z80_ld_regc_port_rega()
{
    memory->writeByte(0xFF00 + registers.C, registers.A);
}


void CPU::z80_ld_reghl_addr16_dat8()
{
    memory->writeByte(registers.HL, memory->readByte(registers.PC));
    registers.PC++;
}


void CPU::z80_ld_reghl_addr16_reg8(uint8_t *reg8)
{
    memory->writeByte(registers.HL, *reg8);
}


void CPU::z80_ld_reghl_sp_add_dat8()
{
    uint16_t data = registers.SP += (int8_t)memory->readByte(registers.PC);
    registers.PC++;

    memory->writeByte(registers.HL, (data & 0xFF));
    memory->writeByte(registers.HL + 1, (data & 0xFF00) >> 8);
}


void CPU::z80_ld_sp_reghl()
{
    registers.SPl = memory->readByte(registers.HL);
    registers.SPh = memory->readByte(registers.HL + 1);
}


void CPU::z80_ldd_rega_reghl_addr16()
{
    registers.A = memory->readByte(registers.HL);
    registers.HL--;
}


void CPU::z80_ldd_reghl_addr16_rega()
{
    memory->writeByte(registers.HL, registers.A);
    registers.HL--;
}


void CPU::z80_ldh_addr8_rega()
{
    memory->writeByte(0xFF00 + memory->readByte(registers.PC), registers.A);
    registers.PC++;
}


void CPU::z80_ldh_rega_addr8()
{
    registers.A = memory->readByte(0xFF00 + memory->readByte(registers.PC));
    registers.PC++;
}


void CPU::z80_ldi_rega_reghl_addr16()
{
    registers.A = memory->readByte(registers.HL);
    registers.HL++;
}


void CPU::z80_ldi_reghl_addr16_rega()
{
    memory->writeByte(registers.HL, registers.A);
    registers.HL++;
}


void CPU::z80_nop()
{

}


void CPU::z80_pop_reg16(uint16_t *reg16)
{
    *reg16 = memory->readByte(registers.SP);
    registers.SP++;
    *reg16 += memory->readByte(registers.SP) << 8;
    registers.SP++;
}


void CPU::z80_push_reg16(uint16_t *reg16)
{
    registers.SP--;
    memory->writeByte(registers.SP, *reg16 >> 8);
    registers.SP--;
    memory->writeByte(registers.SP, (*reg16 & 0x00FF));
}


void CPU::z80_rega_and_dat8()
{
    registers.A &= memory->readByte(registers.PC);
    registers.PC++;

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;
}


void CPU::z80_rega_and_reg8(uint8_t *reg8)
{
    registers.A &= *reg8;

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;
}


void CPU::z80_rega_and_reghl_addr16()
{
    registers.A &= memory->readByte(registers.HL);

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;
}


void CPU::z80_rega_cp_reg8(uint8_t *reg8)
{
    if ((registers.A - *reg8) == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    if ((registers.A - *reg8) < 0)
        registers.flagC = true;
    else
        registers.flagC = false;

    registers.flagN = true;
}


void CPU::z80_rega_cp_reghl_addr16()
{
    uint8_t data = memory->readByte(registers.HL);

    if ((registers.A - data) == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    if ((registers.A - data) < 0)
        registers.flagC = true;
    else
        registers.flagC = false;

    registers.flagN = true;
}


void CPU::z80_rega_cp_dat8()
{
    uint8_t data = memory->readByte(registers.PC);
    registers.PC++;

    if ((registers.A - data) == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    if ((registers.A - data) < 0)
        registers.flagC = true;
    else
        registers.flagC = false;

    registers.flagN = true;
}


void CPU::z80_rega_or_dat8()
{
    registers.A |= memory->readByte(registers.PC);
    registers.PC++;

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;
}


void CPU::z80_rega_or_reg8(uint8_t *reg8)
{
    registers.A |= *reg8;

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;
}


void CPU::z80_rega_or_reghl_addr16()
{
    registers.A |= memory->readByte(registers.HL);

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;
}


void CPU::z80_rega_xor_dat8()
{
    registers.A ^= memory->readByte(registers.PC);
    registers.PC++;

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;
}


void CPU::z80_rega_xor_reg8(uint8_t *reg8)
{
    registers.A ^= *reg8;

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;
}


void CPU::z80_rega_xor_reghl_addr16()
{
    registers.A ^= memory->readByte(registers.HL);

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;
}


void CPU::z80_ret()
{
    z80_pop_reg16(&registers.PC);
}


void CPU::z80_ret_c()
{
    if (registers.flagC == true) {
        z80_pop_reg16(&registers.PC);

        clockCyclesExecuted += 12;
    }
}


void CPU::z80_ret_nc()
{
    if (registers.flagC == false) {
        z80_pop_reg16(&registers.PC);

        clockCyclesExecuted += 12;
    }
}


void CPU::z80_ret_nz()
{
    if (registers.flagZ == false) {
        z80_pop_reg16(&registers.PC);

        clockCyclesExecuted += 12;
    }
}


void CPU::z80_ret_z()
{
    if (registers.flagZ == true) {
        z80_pop_reg16(&registers.PC);

        clockCyclesExecuted += 12;
    }
}


void CPU::z80_reti()
{
    z80_pop_reg16(&registers.PC);

    z80_ei();
}


void CPU::z80_rla()
{
    bool carry = registers.flagC;
    registers.flagC = registers.A & 0x80;
    registers.A = (registers.A << 1) | carry;

    registers.flagZ = false;
    registers.flagN = false;
    registers.flagH = false;
}


void CPU::z80_rlca()
{
    registers.flagC = registers.A & 0x80;
    registers.A = (registers.A << 1) | ((registers.A & 0x80) >> 7);

    registers.flagZ = false;
    registers.flagN = false;
    registers.flagH = false;
}


void CPU::z80_rra()
{
    bool carry = registers.flagC;
    registers.flagC = registers.A & 0x01;
    registers.A = (registers.A >> 1) | (carry << 7);

    registers.flagZ = false;
    registers.flagN = false;
    registers.flagH = false;
}


void CPU::z80_rrca()
{
    registers.flagC = registers.A & 0x01;
    registers.A = (registers.A << 1) | ((registers.A & 0x80) >> 7);

    registers.flagZ = false;
    registers.flagN = false;
    registers.flagH = false;
}


void CPU::z80_rst(uint8_t addr8)
{
    z80_push_reg16(&registers.PC);
    registers.PC = addr8;
}


void CPU::z80_sbc_rega_dat8()
{
    int16_t result = registers.A - (memory->readByte(registers.PC) + registers.flagC);
    registers.PC++;
    registers.A = result;

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    if (result < 0)
        registers.flagC = true;
    else
        registers.flagC = false;
}


void CPU::z80_sbc_rega_reg8(uint8_t *reg8)
{
    int16_t result = registers.A - (*reg8 + registers.flagC);
    registers.A = result;

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    if (result < 0)
        registers.flagC = true;
    else
        registers.flagC = false;
}


void CPU::z80_sbc_rega_reghl_addr16()
{
    int16_t result = registers.A - (memory->readByte(registers.HL) + registers.flagC);
    registers.A = result;

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    if (result < 0)
        registers.flagC = true;
    else
        registers.flagC = false;
}


void CPU::z80_scf()
{
    registers.flagN = false;
    registers.flagH = false;
    registers.flagC = true;
}


void CPU::z80_stop()
{
    stopped = true;
    registers.PC++;
}


void CPU::z80_sub_rega_dat8()
{
    int16_t result = registers.A - memory->readByte(registers.PC);
    registers.PC++;
    registers.A = result;

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    if (result < 0)
        registers.flagC = true;
    else
        registers.flagC = false;
}


void CPU::z80_sub_rega_reg8(uint8_t *reg8)
{
    int16_t result = registers.A - *reg8;
    registers.A = result;

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    if (result < 0)
        registers.flagC = true;
    else
        registers.flagC = false;
}


void CPU::z80_sub_rega_reghl_addr16()
{
    int16_t result = registers.A - memory->readByte(registers.HL);
    registers.A = result;

    if (registers.A == 0)
        registers.flagZ = true;
    else
        registers.flagZ = false;

    if (result < 0)
        registers.flagC = true;
    else
        registers.flagC = false;
}
