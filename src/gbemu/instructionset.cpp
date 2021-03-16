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


void CPU::z80_ld_reg16_addr16_reg8(uint16_t *reg16, uint8_t *reg8) {
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


void CPU::z80_ld_addr16_sp()
{

}


void CPU::z80_add_reghl_reg16(uint16_t *reg16)
{
    registers.HL += *reg16;
}


void CPU::z80_ld_rega_addr16_reg16(uint16_t *reg16)
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
    registers.PC--;
    registers.PC += (int8_t)memory->readByte(registers.PC);
}


void CPU::z80_rra()
{

}


void CPU::z80_jr_nz()
{
    if (registers.flagZ == false) {
        registers.PC--;
        registers.PC = (int8_t)memory->readByte(registers.PC);
        clockCyclesExecuted += 4;
    }
}


void CPU::z80_ldi_reghl_addr16_rega()
{
    memory->writeByte(registers.HL, registers.A);
    registers.HL++;
}


void CPU::z80_daa()
{

}


void CPU::z80_jr_z()
{
    if (registers.flagZ == true) {
        registers.PC--;
        registers.PC = (int8_t)memory->readByte(registers.PC);
        clockCyclesExecuted += 4;
    }
}


void CPU::z80_ldi_rega_reghl_addr16()
{
    registers.A = memory->readByte(registers.HL);
    registers.HL++;
}


void CPU::z80_cpl()
{

}


void CPU::z80_jr_nc()
{
    if (registers.flagC == false) {
        registers.PC--;
        registers.PC = (int8_t)memory->readByte(registers.PC);
        clockCyclesExecuted += 4;
    }
}


void CPU::z80_ldd_reghl_addr16_rega()
{
    memory->writeByte(registers.HL, registers.A);
    registers.HL++;
}


void CPU::z80_inc_reghl_addr16()
{
    memory->writeByte(registers.HL, memory->readByte(registers.HL) + 1);
}


void CPU::z80_dec_reghl_addr16()
{
    memory->writeByte(registers.HL, memory->readByte(registers.HL) - 1);
}


void CPU::z80_ld_reghl_addr16_dat8()
{
    memory->writeByte(registers.HL, memory->readByte(registers.PC));
    registers.PC++;
}


void CPU::z80_scf()
{
    registers.flagN = false;
    registers.flagH = false;
    registers.flagC = true;
}


void CPU::z80_ldd_rega_reghl_addr16()
{
    registers.A = memory->readByte(registers.HL);
    registers.HL++;
}


void CPU::z80_jr_c()
{
    if (registers.flagC == true) {
        registers.PC--;
        registers.PC = (int8_t)memory->readByte(registers.PC);
        clockCyclesExecuted += 4;
    }
}


void CPU::z80_ccf()
{
    registers.flagN = false;
    registers.flagH = false;
    registers.flagC ^= 1;
}


void CPU::z80_ld_reg8_reg8(uint8_t *reg8dest, uint8_t *reg8src)
{
    *reg8dest = *reg8src;
}


void CPU::z80_ld_reg8_reghl_addr16(uint8_t *reg8)
{
    *reg8 = memory->readByte(registers.HL);
}


void CPU::z80_ld_reghl_addr16_reg8(uint8_t *reg8)
{
    memory->writeByte(registers.HL, *reg8);
}


void CPU::z80_halt()
{

}


void CPU::z80_add_rega_reg8(uint8_t *reg8)
{

}


void CPU::z80_add_rega_reghl_addr16()
{

}


void CPU::z80_adc_rega_reg8(uint8_t *reg8)
{

}


void CPU::z80_adc_rega_reghl_addr16()
{

}


void CPU::z80_sub_rega_reg8(uint8_t *reg8)
{

}


void CPU::z80_sub_rega_reghl_addr16()
{

}


void CPU::z80_sbc_rega_reg8(uint8_t *reg8)
{

}


void CPU::z80_sbc_rega_reghl_addr16()
{

}

void CPU::z80_rega_and_reg8(uint8_t *reg8)
{

}

void CPU::z80_rega_and_reghl_addr16()
{

}

void CPU::z80_rega_xor_reg8(uint8_t *reg8)
{

}

void CPU::z80_rega_xor_reghl_addr16()
{

}

void CPU::z80_rega_or_reg8(uint8_t *reg8)
{

}

void CPU::z80_rega_or_reghl_addr16()
{

}

void CPU::z80_rega_cp_reg8(uint8_t *reg8)
{

}

void CPU::z80_rega_cp_reghl_addr16()
{

}

void CPU::z80_ret_nz()
{
    if (registers.flagZ == false) {
        registers.PC = memory->readByte(registers.SP) + (memory->readByte(registers.SP + 1) << 8);
        registers.SP += 2;

        clockCyclesExecuted += 12;
    }
}

void CPU::z80_pop_reg16(uint16_t *reg16)
{

}

void CPU::z80_jp_nz()
{
    if (registers.flagZ == false) {
        registers.PC = memory->readByte(registers.PC) + (memory->readByte(registers.PC + 1) << 8);
        clockCyclesExecuted += 4;
    }
}

void CPU::z80_call_nz()
{
    if (registers.flagZ == false) {
        memory->writeByte(registers.SP, registers.PCl);
        registers.SP--;
        memory->writeByte(registers.SP, registers.PCh);
        registers.SP--;

        registers.PC = memory->readByte(registers.PC) + (memory->readByte(registers.PC + 1) << 8);

        clockCyclesExecuted += 12;
    }
}

void CPU::z80_push_reg16(uint16_t *reg16)
{

}

void CPU::z80_add_rega_dat8()
{

}

void CPU::z80_rst(uint8_t addr8)
{

}

void CPU::z80_ret_z()
{
    if (registers.flagZ == true) {
        registers.PC = memory->readByte(registers.SP) + (memory->readByte(registers.SP + 1) << 8);
        registers.SP += 2;

        clockCyclesExecuted += 12;
    }
}

void CPU::z80_ret()
{
    registers.PC = memory->readByte(registers.SP) + (memory->readByte(registers.SP + 1) << 8);
    registers.SP += 2;
}

void CPU::z80_jp_z()
{
    if (registers.flagZ == true) {
        registers.PC = memory->readByte(registers.PC) + (memory->readByte(registers.PC + 1) << 8);
        clockCyclesExecuted += 4;
    }
}

void CPU::z80_call_z()
{
    if (registers.flagZ == true) {
        memory->writeByte(registers.SP, registers.PCl);
        registers.SP--;
        memory->writeByte(registers.SP, registers.PCh);
        registers.SP--;

        registers.PC = memory->readByte(registers.PC) + (memory->readByte(registers.PC + 1) << 8);

        clockCyclesExecuted += 12;
    }
}

void CPU::z80_call()
{
    memory->writeByte(registers.SP, registers.PCl);
    registers.SP--;
    memory->writeByte(registers.SP, registers.PCh);
    registers.SP--;

    registers.PC = memory->readByte(registers.PC) + (memory->readByte(registers.PC + 1) << 8);
}

void CPU::z80_adc_rega_dat8()
{

}

void CPU::z80_ret_nc()
{
    if (registers.flagC == false) {
        registers.PC = memory->readByte(registers.SP) + (memory->readByte(registers.SP + 1) << 8);
        registers.SP += 2;

        clockCyclesExecuted += 12;
    }
}

void CPU::z80_jp_nc()
{
    if (registers.flagC == false) {
        registers.PC = memory->readByte(registers.PC) + (memory->readByte(registers.PC + 1) << 8);
        clockCyclesExecuted += 4;
    }
}

void CPU::z80_call_nc()
{
    if (registers.flagC == false) {
        memory->writeByte(registers.SP, registers.PCl);
        registers.SP--;
        memory->writeByte(registers.SP, registers.PCh);
        registers.SP--;

        registers.PC = memory->readByte(registers.PC) + (memory->readByte(registers.PC + 1) << 8);

        clockCyclesExecuted += 12;
    }
}

void CPU::z80_ret_c()
{
    if (registers.flagC == true) {
        registers.PC = memory->readByte(registers.SP) + (memory->readByte(registers.SP + 1) << 8);
        registers.SP += 2;

        clockCyclesExecuted += 12;
    }
}

void CPU::z80_reti()
{
    registers.PC = memory->readByte(registers.SP) + (memory->readByte(registers.SP + 1) << 8);
    registers.SP += 2;

    z80_ei();
}

void CPU::z80_jp_c()
{
    if (registers.flagZ == true) {
        registers.PC = memory->readByte(registers.PC) + (memory->readByte(registers.PC + 1) << 8);
        clockCyclesExecuted += 4;
    }
}

void CPU::z80_call_c()
{
    if (registers.flagC == true) {
        memory->writeByte(registers.SP, registers.PCl);
        registers.SP--;
        memory->writeByte(registers.SP, registers.PCh);
        registers.SP--;

        registers.PC = memory->readByte(registers.PC) + (memory->readByte(registers.PC + 1) << 8);

        clockCyclesExecuted += 12;
    }
}

void CPU::z80_sbc_rega_dat8()
{

}

void CPU::z80_ldh_addr8_rega()
{

}

void CPU::z80_ld_regc_port_rega()
{

}

void CPU::z80_rega_and_dat8()
{

}

void CPU::z80_add_sp_dat8()
{

}

void CPU::z80_jp_reghl_addr16()
{

}

void CPU::z80_ld_addr16_rega()
{

}

void CPU::z80_rega_xor_dat8()
{

}

void CPU::z80_ldh_rega_addr8()
{

}

void CPU::z80_ld_rega_regc_port()
{

}

void CPU::z80_di()
{
    interruptMasterEnableFlag = false;
}

void CPU::z80_rega_or_dat8()
{

}

void CPU::z80_ld_reghl_sp_add_dat8()
{

}

void CPU::z80_ld_sp_reghl()
{

}

void CPU::z80_ld_rega_addr16()
{

}

void CPU::z80_ei()
{
    interruptMasterEnableFlag = true;
}

void CPU::z80_rega_cp_dat8()
{

}


void CPU::z80_jp_a16() {
    registers.PC = memory->readByte(registers.PC) + (memory->readByte(registers.PC + 1) << 8);
}
