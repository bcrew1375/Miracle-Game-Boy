#include "cpu.h"
#include "cycletables.h"


CPU::CPU(Memory *memory, IOPorts *ioPorts, Display *display)
{
    this->memory = memory;
    this->ioPorts = ioPorts;
    this->display = display;
    this->resetCPU();
}


void CPU::resetCPU() {

    registers.AF = 0x01B0;
    registers.BC = 0x0013;
    registers.DE = 0x00D8;
    registers.HL = 0x014D;
    registers.SP = 0xFFFE;
    registers.PC = 0x0100; // PC would normally start at 0x0000, but assume the boot rom is not present.

    /*memory->writeByte(0xFF05, 0x00); // TIMA
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
    memory->writeByte(0xFF40, 0x91); // LCD Control
    memory->writeByte(0xFF41, 0x02); // LCD STAT
    memory->writeByte(0xFF42, 0x00); // Scroll Y
    memory->writeByte(0xFF43, 0x00); // Scroll X
    memory->writeByte(0xFF44, 0x99); // LCD Y Coordinate
    memory->writeByte(0xFF45, 0x00); // LCD Y Compare
    memory->writeByte(0xFF47, 0xFC); // BG Palette
    memory->writeByte(0xFF48, 0xFF); // Object Palette 0
    memory->writeByte(0xFF49, 0xFF); // Object Palette 1
    memory->writeByte(0xFF4A, 0x00); // Window Y
    memory->writeByte(0xFF4B, 0x00); // Window X
    memory->writeByte(0xFFFF, 0x00); // Interrupt Enable*/

    interruptMasterEnableFlag = false;
    stopped = false;
    halted = false;
}


uint16_t CPU::getRegisterPC()
{
    return registers.PC;
}


void CPU::handleInterrupts()
{
    uint8_t enabledInterruptFlags = memory->readByte(0xFFFF);
    uint8_t interruptRequestFlags = ioPorts->getInterruptRequestFlags();

    if (interruptMasterEnableFlag == true) {
        // V-Blank
        if (((interruptRequestFlags & 0x01)) && ((enabledInterruptFlags & 0x01))) {
            ioPorts->setInterruptRequestFlags(interruptRequestFlags & 0x1E);
            interruptMasterEnableFlag = false;
            z80_push_reg16(&registers.PC);
            registers.PC = 0x0040;
            halted = false;
        }
        // LCD Status
        else if (((interruptRequestFlags & 0x02)) && ((enabledInterruptFlags & 0x02))) {
            ioPorts->setInterruptRequestFlags(interruptRequestFlags & 0x1D);
            interruptMasterEnableFlag = false;
            z80_push_reg16(&registers.PC);
            registers.PC = 0x0048;
            halted = false;
        }
        // Timer
        else if (((interruptRequestFlags & 0x04)) && ((enabledInterruptFlags & 0x04))) {
            ioPorts->setInterruptRequestFlags(interruptRequestFlags & 0x1B);
            interruptMasterEnableFlag = false;
            z80_push_reg16(&registers.PC);
            registers.PC = 0x0050;
            halted = false;
        }
        // Serial
        else if (((interruptRequestFlags & 0x08)) && ((enabledInterruptFlags & 0x08))) {
            ioPorts->setInterruptRequestFlags(interruptRequestFlags & 0x17);
            interruptMasterEnableFlag = false;
            z80_push_reg16(&registers.PC);
            registers.PC = 0x0058;
            halted = false;
        }
        // Joypad
        else if (((interruptRequestFlags & 0x10)) && ((enabledInterruptFlags & 0x10))) {
            ioPorts->setInterruptRequestFlags(interruptRequestFlags & 0x0F);
            interruptMasterEnableFlag = false;
            z80_push_reg16(&registers.PC);
            registers.PC = 0x0060;
            halted = false;
            stopped = false;
        }
    }
}


uint8_t CPU::getOpcode()
{
    return opcode;
}


bool CPU::getInterruptMasterEnableFlag()
{
    return interruptMasterEnableFlag;
}


void CPU::setInterruptMasterEnableFlag(bool state)
{
    interruptMasterEnableFlag = state;
}


uint32_t CPU::execute(int32_t cyclesLeftToRun) {
    uint8_t cbOpcode;
    uint8_t cbInstructionType;
    uint8_t cbBitNumber;
    uint8_t *cbRegister;

    //if (((registers.PC >= 0x8000) & (registers.PC < 0xC000)) || ((registers.PC > 0xE000) && (registers.PC < 0xFF80)) || (registers.PC == 0xFFFF))
    //    int j = 0;

    //memory->writeByte(0x100, 0xE8);
    //memory->writeByte(0x101, -20);

    while (cyclesLeftToRun > 0) {
        opcode = memory->readByte(registers.PC);
        clockCyclesExecuted = clockCyclesTable[opcode];


        if (registers.PC == 0x02b6)
            int i = 0;

        if (stopped == false) {
            if (halted == false)
                registers.PC++;
            else if ((halted == true) && (interruptMasterEnableFlag == false) && ((ioPorts->getInterruptRequestFlags() & 0x1F) != 0x00) && ((memory->readByte(0xFFFF) & 0x1F) != 0x00)) {
                halted = false; // This accounts for the HALT bug which stops the Program Counter from incrementing for one instruction
                                // if HALT is used with IME disabled while there are interrupts enabled in IE and/or pending. Otherwise, the system halts permanently.
            }
        }

        else {
            // TO DO: Handle stopped condition.
        }


        switch (opcode) {
        case 0x00: z80_nop(); break;
        case 0x01: z80_ld_reg16_dat16(&registers.BC); break;
        case 0x02: z80_ld_reg16_addr16_reg8(&registers.BC, &registers.A); break;
        case 0x03: z80_inc_reg16(&registers.BC); break;
        case 0x04: z80_inc_reg8(&registers.B); break;
        case 0x05: z80_dec_reg8(&registers.B); break;
        case 0x06: z80_ld_reg8_dat8(&registers.B); break;
        case 0x07: z80_rlca(); break;
        case 0x08: z80_ld_addr16_sp(); break;
        case 0x09: z80_add_reghl_reg16(&registers.BC); break;
        case 0x0A: z80_ld_rega_addr16_reg16(&registers.BC); break;
        case 0x0B: z80_dec_reg16(&registers.BC); break;
        case 0x0C: z80_inc_reg8(&registers.C); break;
        case 0x0D: z80_dec_reg8(&registers.C); break;
        case 0x0E: z80_ld_reg8_dat8(&registers.C); break;
        case 0x0F: z80_rrca(); break;
        case 0x10: z80_stop(); break;
        case 0x11: z80_ld_reg16_dat16(&registers.DE); break;
        case 0x12: z80_ld_reg16_addr16_reg8(&registers.DE, &registers.A); break;
        case 0x13: z80_inc_reg16(&registers.DE); break;
        case 0x14: z80_inc_reg8(&registers.D); break;
        case 0x15: z80_dec_reg8(&registers.D); break;
        case 0x16: z80_ld_reg8_dat8(&registers.D); break;
        case 0x17: z80_rla(); break;
        case 0x18: z80_jr_offset(); break;
        case 0x19: z80_add_reghl_reg16(&registers.DE); break;
        case 0x1A: z80_ld_rega_addr16_reg16(&registers.DE); break;
        case 0x1B: z80_dec_reg16(&registers.DE); break;
        case 0x1C: z80_inc_reg8(&registers.E); break;
        case 0x1D: z80_dec_reg8(&registers.E); break;
        case 0x1E: z80_ld_reg8_dat8(&registers.E); break;
        case 0x1F: z80_rra(); break;
        case 0x20: z80_jr_nz(); break;
        case 0x21: z80_ld_reg16_dat16(&registers.HL); break;
        case 0x22: z80_ldi_reghl_addr16_rega(); break;
        case 0x23: z80_inc_reg16(&registers.HL); break;
        case 0x24: z80_inc_reg8(&registers.H); break;
        case 0x25: z80_dec_reg8(&registers.H); break;
        case 0x26: z80_ld_reg8_dat8(&registers.H); break;
        case 0x27: z80_daa(); break;
        case 0x28: z80_jr_z(); break;
        case 0x29: z80_add_reghl_reg16(&registers.HL); break;
        case 0x2A: z80_ldi_rega_reghl_addr16(); break;
        case 0x2B: z80_dec_reg16(&registers.HL); break;
        case 0x2C: z80_inc_reg8(&registers.L); break;
        case 0x2D: z80_dec_reg8(&registers.L); break;
        case 0x2E: z80_ld_reg8_dat8(&registers.L); break;
        case 0x2F: z80_cpl(); break;
        case 0x30: z80_jr_nc(); break;
        case 0x31: z80_ld_reg16_dat16(&registers.SP); break;
        case 0x32: z80_ldd_reghl_addr16_rega(); break;
        case 0x33: z80_inc_reg16(&registers.SP); break;
        case 0x34: z80_inc_reghl_addr16(); break;
        case 0x35: z80_dec_reghl_addr16(); break;
        case 0x36: z80_ld_reghl_addr16_dat8(); break;
        case 0x37: z80_scf(); break;
        case 0x38: z80_jr_c(); break;
        case 0x39: z80_add_reghl_reg16(&registers.SP); break;
        case 0x3A: z80_ldd_rega_reghl_addr16(); break;
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
        case 0x46: z80_ld_reg8_reghl_addr16(&registers.B); break;
        case 0x47: z80_ld_reg8_reg8(&registers.B, &registers.A); break;
        case 0x48: z80_ld_reg8_reg8(&registers.C, &registers.B); break;
        case 0x49: z80_ld_reg8_reg8(&registers.C, &registers.C); break;
        case 0x4A: z80_ld_reg8_reg8(&registers.C, &registers.D); break;
        case 0x4B: z80_ld_reg8_reg8(&registers.C, &registers.E); break;
        case 0x4C: z80_ld_reg8_reg8(&registers.C, &registers.H); break;
        case 0x4D: z80_ld_reg8_reg8(&registers.C, &registers.L); break;
        case 0x4E: z80_ld_reg8_reghl_addr16(&registers.C); break;
        case 0x4F: z80_ld_reg8_reg8(&registers.C, &registers.A); break;
        case 0x50: z80_ld_reg8_reg8(&registers.D, &registers.B); break;
        case 0x51: z80_ld_reg8_reg8(&registers.D, &registers.C); break;
        case 0x52: z80_ld_reg8_reg8(&registers.D, &registers.D); break;
        case 0x53: z80_ld_reg8_reg8(&registers.D, &registers.E); break;
        case 0x54: z80_ld_reg8_reg8(&registers.D, &registers.H); break;
        case 0x55: z80_ld_reg8_reg8(&registers.D, &registers.L); break;
        case 0x56: z80_ld_reg8_reghl_addr16(&registers.D); break;
        case 0x57: z80_ld_reg8_reg8(&registers.D, &registers.A); break;
        case 0x58: z80_ld_reg8_reg8(&registers.E, &registers.B); break;
        case 0x59: z80_ld_reg8_reg8(&registers.E, &registers.C); break;
        case 0x5A: z80_ld_reg8_reg8(&registers.E, &registers.D); break;
        case 0x5B: z80_ld_reg8_reg8(&registers.E, &registers.E); break;
        case 0x5C: z80_ld_reg8_reg8(&registers.E, &registers.H); break;
        case 0x5D: z80_ld_reg8_reg8(&registers.E, &registers.L); break;
        case 0x5E: z80_ld_reg8_reghl_addr16(&registers.E); break;
        case 0x5F: z80_ld_reg8_reg8(&registers.E, &registers.A); break;
        case 0x60: z80_ld_reg8_reg8(&registers.H, &registers.B); break;
        case 0x61: z80_ld_reg8_reg8(&registers.H, &registers.C); break;
        case 0x62: z80_ld_reg8_reg8(&registers.H, &registers.D); break;
        case 0x63: z80_ld_reg8_reg8(&registers.H, &registers.E); break;
        case 0x64: z80_ld_reg8_reg8(&registers.H, &registers.H); break;
        case 0x65: z80_ld_reg8_reg8(&registers.H, &registers.L); break;
        case 0x66: z80_ld_reg8_reghl_addr16(&registers.H); break;
        case 0x67: z80_ld_reg8_reg8(&registers.H, &registers.A); break;
        case 0x68: z80_ld_reg8_reg8(&registers.L, &registers.B); break;
        case 0x69: z80_ld_reg8_reg8(&registers.L, &registers.C); break;
        case 0x6A: z80_ld_reg8_reg8(&registers.L, &registers.D); break;
        case 0x6B: z80_ld_reg8_reg8(&registers.L, &registers.E); break;
        case 0x6C: z80_ld_reg8_reg8(&registers.L, &registers.H); break;
        case 0x6D: z80_ld_reg8_reg8(&registers.L, &registers.L); break;
        case 0x6E: z80_ld_reg8_reghl_addr16(&registers.L); break;
        case 0x6F: z80_ld_reg8_reg8(&registers.L, &registers.A); break;
        case 0x70: z80_ld_reghl_addr16_reg8(&registers.B); break;
        case 0x71: z80_ld_reghl_addr16_reg8(&registers.C); break;
        case 0x72: z80_ld_reghl_addr16_reg8(&registers.D); break;
        case 0x73: z80_ld_reghl_addr16_reg8(&registers.E); break;
        case 0x74: z80_ld_reghl_addr16_reg8(&registers.H); break;
        case 0x75: z80_ld_reghl_addr16_reg8(&registers.L); break;
        case 0x76: z80_halt(); break;
        case 0x77: z80_ld_reghl_addr16_reg8(&registers.A); break;
        case 0x78: z80_ld_reg8_reg8(&registers.A, &registers.B); break;
        case 0x79: z80_ld_reg8_reg8(&registers.A, &registers.C); break;
        case 0x7A: z80_ld_reg8_reg8(&registers.A, &registers.D); break;
        case 0x7B: z80_ld_reg8_reg8(&registers.A, &registers.E); break;
        case 0x7C: z80_ld_reg8_reg8(&registers.A, &registers.H); break;
        case 0x7D: z80_ld_reg8_reg8(&registers.A, &registers.L); break;
        case 0x7E: z80_ld_reg8_reghl_addr16(&registers.A); break;
        case 0x7F: z80_ld_reg8_reg8(&registers.A, &registers.A); break;
        case 0x80: z80_add_rega_reg8(&registers.B); break;
        case 0x81: z80_add_rega_reg8(&registers.C); break;
        case 0x82: z80_add_rega_reg8(&registers.D); break;
        case 0x83: z80_add_rega_reg8(&registers.E); break;
        case 0x84: z80_add_rega_reg8(&registers.H); break;
        case 0x85: z80_add_rega_reg8(&registers.L); break;
        case 0x86: z80_add_rega_reghl_addr16(); break;
        case 0x87: z80_add_rega_reg8(&registers.A); break;
        case 0x88: z80_adc_rega_reg8(&registers.B); break;
        case 0x89: z80_adc_rega_reg8(&registers.C); break;
        case 0x8A: z80_adc_rega_reg8(&registers.D); break;
        case 0x8B: z80_adc_rega_reg8(&registers.E); break;
        case 0x8C: z80_adc_rega_reg8(&registers.H); break;
        case 0x8D: z80_adc_rega_reg8(&registers.L); break;
        case 0x8E: z80_adc_rega_reghl_addr16(); break;
        case 0x8F: z80_adc_rega_reg8(&registers.A); break;
        case 0x90: z80_sub_rega_reg8(&registers.B); break;
        case 0x91: z80_sub_rega_reg8(&registers.C); break;
        case 0x92: z80_sub_rega_reg8(&registers.D); break;
        case 0x93: z80_sub_rega_reg8(&registers.E); break;
        case 0x94: z80_sub_rega_reg8(&registers.H); break;
        case 0x95: z80_sub_rega_reg8(&registers.L); break;
        case 0x96: z80_sub_rega_reghl_addr16(); break;
        case 0x97: z80_sub_rega_reg8(&registers.A); break;
        case 0x98: z80_sbc_rega_reg8(&registers.B); break;
        case 0x99: z80_sbc_rega_reg8(&registers.C); break;
        case 0x9A: z80_sbc_rega_reg8(&registers.D); break;
        case 0x9B: z80_sbc_rega_reg8(&registers.E); break;
        case 0x9C: z80_sbc_rega_reg8(&registers.H); break;
        case 0x9D: z80_sbc_rega_reg8(&registers.L); break;
        case 0x9E: z80_sbc_rega_reghl_addr16(); break;
        case 0x9F: z80_sbc_rega_reg8(&registers.A); break;
        case 0xA0: z80_rega_and_reg8(&registers.B); break;
        case 0xA1: z80_rega_and_reg8(&registers.C); break;
        case 0xA2: z80_rega_and_reg8(&registers.D); break;
        case 0xA3: z80_rega_and_reg8(&registers.E); break;
        case 0xA4: z80_rega_and_reg8(&registers.H); break;
        case 0xA5: z80_rega_and_reg8(&registers.L); break;
        case 0xA6: z80_rega_and_reghl_addr16(); break;
        case 0xA7: z80_rega_and_reg8(&registers.A); break;
        case 0xA8: z80_rega_xor_reg8(&registers.B); break;
        case 0xA9: z80_rega_xor_reg8(&registers.C); break;
        case 0xAA: z80_rega_xor_reg8(&registers.D); break;
        case 0xAB: z80_rega_xor_reg8(&registers.E); break;
        case 0xAC: z80_rega_xor_reg8(&registers.H); break;
        case 0xAD: z80_rega_xor_reg8(&registers.L); break;
        case 0xAE: z80_rega_xor_reghl_addr16(); break;
        case 0xAF: z80_rega_xor_reg8(&registers.A); break;
        case 0xB0: z80_rega_or_reg8(&registers.B); break;
        case 0xB1: z80_rega_or_reg8(&registers.C); break;
        case 0xB2: z80_rega_or_reg8(&registers.D); break;
        case 0xB3: z80_rega_or_reg8(&registers.E); break;
        case 0xB4: z80_rega_or_reg8(&registers.H); break;
        case 0xB5: z80_rega_or_reg8(&registers.L); break;
        case 0xB6: z80_rega_or_reghl_addr16(); break;
        case 0xB7: z80_rega_or_reg8(&registers.A); break;
        case 0xB8: z80_rega_cp_reg8(&registers.B); break;
        case 0xB9: z80_rega_cp_reg8(&registers.C); break;
        case 0xBA: z80_rega_cp_reg8(&registers.D); break;
        case 0xBB: z80_rega_cp_reg8(&registers.E); break;
        case 0xBC: z80_rega_cp_reg8(&registers.H); break;
        case 0xBD: z80_rega_cp_reg8(&registers.L); break;
        case 0xBE: z80_rega_cp_reghl_addr16(); break;
        case 0xBF: z80_rega_cp_reg8(&registers.A); break;
        case 0xC0: z80_ret_nz(); break;
        case 0xC1: z80_pop_reg16(&registers.BC); break;
        case 0xC2: z80_jp_nz(); break;
        case 0xC3: z80_jp_a16(); break;
        case 0xC4: z80_call_nz(); break;
        case 0xC5: z80_push_reg16(&registers.BC); break;
        case 0xC6: z80_add_rega_dat8(); break;
        case 0xC7: z80_rst(0x00); break;
        case 0xC8: z80_ret_z(); break;
        case 0xC9: z80_ret(); break;
        case 0xCA: z80_jp_z(); break;
        // Handle the extended CB bit manipulation instructions.
        case 0xCB: {
            cbOpcode = memory->readByte(registers.PC);
            clockCyclesExecuted += clockCyclesCBTable[cbOpcode];

            // Decode the opcode's instruction type, bits 7-6, where b00 = rotate/shift, b01 = test bit, b10 = reset bit, b11 = set bit
            cbInstructionType = cbOpcode & 0xC0;

            // Decode the bit that will be operated on, bits 5-3, where b000 = bit 0, b001 = bit 1, b010 = bit 2, b011 = bit 3, b100 = bit 4, b101 = bit 5, b110 = bit 6, b111 = bit 7
            cbBitNumber = cbOpcode & 0x38;

            // Decode the register number to operate on, bits 2-0, where b000 = B, b001 = C, b010 = D, b011 = E, b100 = H, b101 = L, b110 = (HL), b111 = A
            switch (cbOpcode & 0b00000111) {
            case 0b00000000: cbRegister = &registers.B; break;
            case 0b00000001: cbRegister = &registers.C; break;
            case 0b00000010: cbRegister = &registers.D; break;
            case 0b00000011: cbRegister = &registers.E; break;
            case 0b00000100: cbRegister = &registers.H; break;
            case 0b00000101: cbRegister = &registers.L; break;
            case 0b00000110: cbRegister = nullptr; break; // nullptr represents (HL), which will be handled by its own set of functions.
            case 0b00000111: cbRegister = &registers.A; break;
            }

            if (cbRegister != nullptr) {
                switch (cbInstructionType) {
                // This is a rotate/shift instruction. In this case, the bit number serves as an index for the operation to perform where
                //                          b000 = rlc, b001 = rrc, b010 = rl, b011 = rr, b100 = sla, b101 = sra, b110 = swap, b111 = srl
                case 0b00000000: {
                    switch (cbBitNumber) {
                    case 0b00000000: z80_cb_rlc_reg8(cbRegister); break;
                    case 0b00001000: z80_cb_rrc_reg8(cbRegister); break;
                    case 0b00010000: z80_cb_rl_reg8(cbRegister); break;
                    case 0b00011000: z80_cb_rr_reg8(cbRegister); break;
                    case 0b00100000: z80_cb_sla_reg8(cbRegister); break;
                    case 0b00101000: z80_cb_sra_reg8(cbRegister); break;
                    case 0b00110000: z80_cb_swap_reg8(cbRegister); break;
                    case 0b00111000: z80_cb_srl_reg8(cbRegister); break;
                    }
                } break;
                case 0b01000000: {
                    // Create a mask to test the bit with.
                    cbBitNumber >>= 3;
                    cbBitNumber = 1 << cbBitNumber;

                    z80_cb_test_reg8_bit(cbRegister, cbBitNumber);
                } break;
                case 0b10000000: {
                    // Create a mask to reset the bit with.
                    cbBitNumber >>= 3;
                    cbBitNumber = 0xFF - (1 << cbBitNumber);

                    z80_cb_reset_reg8_bit(cbRegister, cbBitNumber);
                } break;
                case 0b11000000: {
                    // Create a mask to set the bit with.
                    cbBitNumber >>= 3;
                    cbBitNumber = 1 << cbBitNumber;

                    z80_cb_set_reg8_bit(cbRegister, cbBitNumber);
                } break;
                }
            }
            // Handle (HL) cases.
            else {
                switch (cbInstructionType) {
                case 0b00000000: {
                    switch (cbBitNumber) {
                    case 0b00000000: z80_cb_rlc_reghl_addr16(); break;
                    case 0b00001000: z80_cb_rrc_reghl_addr16(); break;
                    case 0b00010000: z80_cb_rl_reghl_addr16(); break;
                    case 0b00011000: z80_cb_rr_reghl_addr16(); break;
                    case 0b00100000: z80_cb_sla_reghl_addr16(); break;
                    case 0b00101000: z80_cb_sra_reghl_addr16(); break;
                    case 0b00110000: z80_cb_swap_reghl_addr16(); break;
                    case 0b00111000: z80_cb_srl_reghl_addr16(); break;
                    }
                } break;
                case 0b01000000: {
                    // Create a mask to test the bit with.
                    cbBitNumber >>= 3;
                    cbBitNumber = 1 << cbBitNumber;

                    z80_cb_test_reghl_addr16_bit(cbBitNumber);
                } break;
                case 0b10000000: {
                    // Create a mask to reset the bit with.
                    cbBitNumber >>= 3;
                    cbBitNumber = 0xFF - (1 << cbBitNumber);

                    z80_cb_reset_reghl_addr16_bit(cbBitNumber);
                } break;
                case 0b11000000: {
                    // Create a mask to set the bit with.
                    cbBitNumber >>= 3;
                    cbBitNumber = 1 << cbBitNumber;

                    z80_cb_set_reghl_addr16_bit(cbBitNumber);
                } break;
                }
            }

            registers.PC++;
        } break;
        case 0xCC: z80_call_z(); break;
        case 0xCD: z80_call_a16(); break;
        case 0xCE: z80_adc_rega_dat8(); break;
        case 0xCF: z80_rst(0x08); break;
        case 0xD0: z80_ret_nc(); break;
        case 0xD1: z80_pop_reg16(&registers.DE); break;
        case 0xD2: z80_jp_nc(); break;
        case 0xD3: return 0; break;
        case 0xD4: z80_call_nc(); break;
        case 0xD5: z80_push_reg16(&registers.DE); break;
        case 0xD6: z80_sub_rega_dat8(); break;
        case 0xD7: z80_rst(0x10); break;
        case 0xD8: z80_ret_c(); break;
        case 0xD9: z80_reti(); break;
        case 0xDA: z80_jp_c(); break;
        case 0xDB: return 0; break;
        case 0xDC: z80_call_c(); break;
        case 0xDD: return 0; break;
        case 0xDE: z80_sbc_rega_dat8(); break;
        case 0xDF: z80_rst(0x18); break;
        case 0xE0: z80_ldh_addr8_rega(); break;
        case 0xE1: z80_pop_reg16(&registers.HL); break;
        case 0xE2: z80_ld_regc_port_rega(); break;
        case 0xE3: return 0; break;
        case 0xE4: return 0; break;
        case 0xE5: z80_push_reg16(&registers.HL); break;
        case 0xE6: z80_rega_and_dat8(); break;
        case 0xE7: z80_rst(0x20); break;
        case 0xE8: z80_add_sp_dat8(); break;
        case 0xE9: z80_jp_reghl(); break;
        case 0xEA: z80_ld_addr16_rega(); break;
        case 0xEB: return 0; break;
        case 0xEC: return 0; break;
        case 0xED: return 0; break;
        case 0xEE: z80_rega_xor_dat8(); break;
        case 0xEF: z80_rst(0x28); break;
        case 0xF0: z80_ldh_rega_addr8(); break;
        case 0xF1: z80_pop_reg16(&registers.AF); break;
        case 0xF2: z80_ld_rega_regc_port(); break;
        case 0xF3: z80_di(); break;
        case 0xF4: return 0; break;
        case 0xF5: z80_push_reg16(&registers.AF); break;
        case 0xF6: z80_rega_or_dat8(); break;
        case 0xF7: z80_rst(0x30); break;
        case 0xF8: z80_ld_reghl_sp_add_dat8(); break;
        case 0xF9: z80_ld_sp_reghl(); break;
        case 0xFA: z80_ld_rega_addr16(); break;
        case 0xFB: z80_ei(); break;
        case 0xFC: return 0; break;
        case 0xFD: return 0; break;
        case 0xFE: z80_rega_cp_dat8(); break;
        case 0xFF: z80_rst(0x38); break;
        }

        ioPorts->updateRegisters(clockCyclesExecuted);

        if (ioPorts->getHBlankBeginFlag() == true)
            display->createScanline();

        handleInterrupts();

        if (clockCyclesExecuted > 0)
            cyclesLeftToRun -= clockCyclesExecuted;
        else
            return cyclesLeftToRun;
    }

    return cyclesLeftToRun;
}
