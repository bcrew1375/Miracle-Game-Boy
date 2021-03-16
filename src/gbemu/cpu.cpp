#include "cpu.h"
#include "cycletables.h"



CPU::CPU(Memory *memory)
{
    this->memory = memory;
    this->resetCPU();
}


void CPU::resetCPU() {

    registers.AF = 0x01B0;
    registers.BC = 0x0013;
    registers.DE = 0x00D8;
    registers.HL = 0x014D;
    registers.SP = 0xFFFE;
    registers.PC = 0x0100; // PC would normally start at 0x0000, but assume the boot rom is not present.

    memory->writeByte(0xFF05, 0x00); // TIMA
    memory->writeByte(0xFF06, 0x00); // TMA
    memory->writeByte(0xFF07, 0x00); // TAC
    memory->writeByte(0xFF10, 0x80); // NR10
    memory->writeByte(0xFF11, 0xBF); // NR11
    memory->writeByte(0xFF14, 0xBF); // NR14
    memory->writeByte(0xFF16, 0x3F); // NR21
    memory->writeByte(0xFF17, 0x00); // NR22
    memory->writeByte(0xFF19, 0xBF); // NR24
    memory->writeByte(0xFF1A, 0x7F); // NR30
    memory->writeByte(0xFF1B, 0xFF); // NR31
    memory->writeByte(0xFF1C, 0x9F); // NR32
    memory->writeByte(0xFF1E, 0xBF); // NR34
    memory->writeByte(0xFF20, 0xFF); // NR41
    memory->writeByte(0xFF21, 0x00); // NR42
    memory->writeByte(0xFF22, 0x00); // NR43
    memory->writeByte(0xFF23, 0xBF); // NR44
    memory->writeByte(0xFF24, 0x77); // NR50
    memory->writeByte(0xFF25, 0xF3); // NR51
    memory->writeByte(0xFF26, 0xF1); // [$FF26] = $F1-GB, $F0-SGB ; NR52
    memory->writeByte(0xFF40, 0x91); // LCDC
    memory->writeByte(0xFF42, 0x00); // SCY
    memory->writeByte(0xFF43, 0x00); // SCX
    memory->writeByte(0xFF45, 0x00); // LYC
    memory->writeByte(0xFF47, 0xFC); // BGP
    memory->writeByte(0xFF48, 0xFF); // OBP0
    memory->writeByte(0xFF49, 0xFF); // OBP1
    memory->writeByte(0xFF4A, 0x00); // WY
    memory->writeByte(0xFF4B, 0x00); // WX
    memory->writeByte(0xFFFF, 0x00); // IE
}


uint32_t CPU::execute() {
    opcode = memory->readByte(registers.PC);
    clockCyclesExecuted = clockCyclesTable[opcode];
    opcode = 0x04;

    registers.PC++;

    switch (opcode) {
    case 0x00: z80_nop(); break;
    case 0x01: z80_ld_reg16_dat16(&registers.BC); break;
    case 0x02: z80_ld_reg16_addr_reg8(&registers.BC, &registers.A); break;
    case 0x03: z80_inc_reg16(&registers.BC); break;
    case 0x04: z80_inc_reg8(&registers.B); break;
    case 0x05: z80_dec_reg8(&registers.B); break;
    case 0x06: z80_ld_reg8_dat8(&registers.B); break;
    case 0x07: z80_rlca(); break;
    case 0x08: z80_ld_addr_sp(); break;
    case 0x09: z80_add_reghl_reg16(&registers.BC); break;
    case 0x0A: z80_ld_rega_addr_reg16(&registers.BC); break;
    case 0x0B: z80_dec_reg16(&registers.BC); break;
    case 0x0C: z80_inc_reg8(&registers.C); break;
    case 0x0D: z80_dec_reg8(&registers.C); break;
    case 0x0E: z80_ld_reg8_dat8(&registers.C); break;
    case 0x0F: z80_rrca(); break;
    case 0x10: z80_stop(); break;
    case 0x11: z80_ld_reg16_dat16(&registers.DE); break;
    case 0x12: z80_ld_reg16_addr_reg8(&registers.DE, &registers.A); break;
    case 0x13: z80_inc_reg16(&registers.DE); break;
    case 0x14: z80_inc_reg8(&registers.D); break;
    case 0x15: z80_dec_reg8(&registers.D); break;
    case 0x16: z80_ld_reg8_dat8(&registers.D); break;
    case 0x17: z80_rla(); break;
    case 0x18: z80_jr_offset(); break;
    case 0x19: z80_add_reghl_reg16(&registers.DE); break;
    case 0x1A: z80_ld_rega_addr_reg16(&registers.DE); break;
    case 0x1B: z80_dec_reg16(&registers.DE); break;
    case 0x1C: z80_inc_reg8(&registers.E); break;
    case 0x1D: z80_dec_reg8(&registers.E); break;
    case 0x1E: z80_ld_reg8_dat8(&registers.E); break;
    case 0x1F: z80_rra(); break;
    case 0x20: z80_jr_nz(); break;
    case 0x21: z80_ld_reg16_dat16(&registers.HL); break;
    case 0x22: z80_ldi_reghl_addr_rega(); break;
    case 0x23: z80_inc_reg16(&registers.HL); break;
    case 0x24: z80_inc_reg8(&registers.H); break;
    case 0x25: z80_dec_reg8(&registers.H); break;
    case 0x26: z80_ld_reg8_dat8(&registers.H); break;
    case 0x27: z80_daa(); break;
    case 0x28: z80_jr_z(); break;
    case 0x29: z80_add_reghl_reg16(&registers.HL); break;
    case 0x2A: z80_ldi_rega_reghl_addr(); break;
    case 0x2B: z80_dec_reg16(&registers.HL); break;
    case 0x2C: z80_inc_reg8(&registers.L); break;
    case 0x2D: z80_dec_reg8(&registers.L); break;
    case 0x2E: z80_ld_reg8_dat8(&registers.L); break;
    case 0x2F: z80_cpl(); break;
    case 0x30: z80_jr_nc(); break;
    case 0x31: z80_ld_reg16_dat16(&registers.SP); break;
    case 0x32: z80_ldd_reghl_addr_rega(); break;
    case 0x33: z80_inc_reg16(&registers.SP); break;
    case 0x34: z80_inc_reghl_addr(); break;
    case 0x35: z80_dec_reghl_addr(); break;
    case 0x36: z80_ld_reghl_addr_dat8(); break;
    case 0x37: z80_scf(); break;
    case 0x38: z80_jr_c(); break;
    case 0x39: z80_add_reghl_reg16(&registers.SP); break;
    case 0x3A: z80_ldd_rega_reghl_addr(); break;
    case 0x3B: z80_dec_reg16(&registers.SP); break;
    case 0x3C: z80_inc_reg8(&registers.A); break;
    case 0x3D: z80_dec_reg8(&registers.A); break;
    case 0x3E: z80_ld_reg8_dat8(&registers.A); break;
    case 0x3F: z80_ccf(); break;
    case 0x40: z80_ld_reg8_reg8(&registers.B, &registers.B); break;
    case 0x41: z80_ld_reg8_reg8(&registers.B, &registers.C); break;
    case 0x42: z80_ld_reg8_reg8(&registers.B, &registers.D); break;
    case 0x43: z80_ld_reg8_reg8(&registers.B, &registers.E); break;
    case 0x44: z80_ld_reg8_reg8(&registers.B, &registers.H); break;
    case 0x45: z80_ld_reg8_reg8(&registers.B, &registers.L); break;
    case 0x46: z80_ld_reg8_reghl_addr(&registers.B); break;
    case 0x47: z80_ld_reg8_reg8(&registers.B, &registers.A); break;
    case 0x48: z80_ld_reg8_reg8(&registers.C, &registers.B); break;
    case 0x49: z80_ld_reg8_reg8(&registers.C, &registers.C); break;
    case 0x4A: z80_ld_reg8_reg8(&registers.C, &registers.D); break;
    case 0x4B: z80_ld_reg8_reg8(&registers.C, &registers.E); break;
    case 0x4C: z80_ld_reg8_reg8(&registers.C, &registers.H); break;
    case 0x4D: z80_ld_reg8_reg8(&registers.C, &registers.L); break;
    case 0x4E: z80_ld_reg8_reghl_addr(&registers.C); break;
    case 0x4F: z80_ld_reg8_reg8(&registers.C, &registers.A); break;
    case 0x50: z80_ld_reg8_reg8(&registers.D, &registers.B); break;
    case 0x51: z80_ld_reg8_reg8(&registers.D, &registers.C); break;
    case 0x52: z80_ld_reg8_reg8(&registers.D, &registers.D); break;
    case 0x53: z80_ld_reg8_reg8(&registers.D, &registers.E); break;
    case 0x54: z80_ld_reg8_reg8(&registers.D, &registers.H); break;
    case 0x55: z80_ld_reg8_reg8(&registers.D, &registers.L); break;
    case 0x56: z80_ld_reg8_reghl_addr(&registers.D); break;
    case 0x57: z80_ld_reg8_reg8(&registers.D, &registers.A); break;
    case 0x58: z80_ld_reg8_reg8(&registers.E, &registers.B); break;
    case 0x59: z80_ld_reg8_reg8(&registers.E, &registers.C); break;
    case 0x5A: z80_ld_reg8_reg8(&registers.E, &registers.D); break;
    case 0x5B: z80_ld_reg8_reg8(&registers.E, &registers.E); break;
    case 0x5C: z80_ld_reg8_reg8(&registers.E, &registers.H); break;
    case 0x5D: z80_ld_reg8_reg8(&registers.E, &registers.L); break;
    case 0x5E: z80_ld_reg8_reghl_addr(&registers.E); break;
    case 0x5F: z80_ld_reg8_reg8(&registers.E, &registers.A); break;
    case 0x60: z80_ld_reg8_reg8(&registers.H, &registers.B); break;
    case 0x61: z80_ld_reg8_reg8(&registers.H, &registers.C); break;
    case 0x62: z80_ld_reg8_reg8(&registers.H, &registers.D); break;
    case 0x63: z80_ld_reg8_reg8(&registers.H, &registers.E); break;
    case 0x64: z80_ld_reg8_reg8(&registers.H, &registers.H); break;
    case 0x65: z80_ld_reg8_reg8(&registers.H, &registers.L); break;
    case 0x66: z80_ld_reg8_reghl_addr(&registers.H); break;
    case 0x67: z80_ld_reg8_reg8(&registers.H, &registers.A); break;
    case 0x68: z80_ld_reg8_reg8(&registers.L, &registers.B); break;
    case 0x69: z80_ld_reg8_reg8(&registers.L, &registers.C); break;
    case 0x6A: z80_ld_reg8_reg8(&registers.L, &registers.D); break;
    case 0x6B: z80_ld_reg8_reg8(&registers.L, &registers.E); break;
    case 0x6C: z80_ld_reg8_reg8(&registers.L, &registers.H); break;
    case 0x6D: z80_ld_reg8_reg8(&registers.L, &registers.L); break;
    case 0x6E: z80_ld_reg8_reghl_addr(&registers.L); break;
    case 0x6F: z80_ld_reg8_reg8(&registers.L, &registers.A); break;
    case 0x70: z80_ld_reghl_addr_reg8(&registers.B);
    case 0x71: z80_ld_reghl_addr_reg8(&registers.C);
    case 0x72: z80_ld_reghl_addr_reg8(&registers.D);
    case 0x73: z80_ld_reghl_addr_reg8(&registers.E);
    case 0x74: z80_ld_reghl_addr_reg8(&registers.H);
    case 0x75: z80_ld_reghl_addr_reg8(&registers.L);
    case 0x76: z80_halt();
    case 0x77: z80_ld_reghl_addr_reg8(&registers.A); break;
    case 0x78: z80_ld_reg8_reg8(&registers.A, &registers.B); break;
    case 0x79: z80_ld_reg8_reg8(&registers.A, &registers.C); break;
    case 0x7A: z80_ld_reg8_reg8(&registers.A, &registers.D); break;
    case 0x7B: z80_ld_reg8_reg8(&registers.A, &registers.E); break;
    case 0x7C: z80_ld_reg8_reg8(&registers.A, &registers.H); break;
    case 0x7D: z80_ld_reg8_reg8(&registers.A, &registers.L); break;
    case 0x7E: z80_ld_reg8_reghl_addr(&registers.A); break;
    case 0x7F: z80_ld_reg8_reg8(&registers.A, &registers.A); break;
    case 0x80: z80_add_rega_reg8(&registers.B); break;
    case 0x81: z80_add_rega_reg8(&registers.C); break;
    case 0x82: z80_add_rega_reg8(&registers.D); break;
    case 0x83: z80_add_rega_reg8(&registers.E); break;
    case 0x84: z80_add_rega_reg8(&registers.H); break;
    case 0x85: z80_add_rega_reg8(&registers.L); break;
    case 0x86: z80_add_rega_reghl_addr(); break;
    case 0x87: z80_add_rega_reg8(&registers.A); break;
    case 0x88: z80_adc_rega_reg8(&registers.B); break;
    case 0x89: z80_adc_rega_reg8(&registers.C); break;
    case 0x8A: z80_adc_rega_reg8(&registers.D); break;
    case 0x8B: z80_adc_rega_reg8(&registers.E); break;
    case 0x8C: z80_adc_rega_reg8(&registers.H); break;
    case 0x8D: z80_adc_rega_reg8(&registers.L); break;
    case 0x8E: z80_adc_rega_reghl_addr(); break;
    case 0x8F: z80_adc_rega_reg8(&registers.A); break;

    case 0xC3: z80_jp_a16(); break;
    }

    return 4;
}
