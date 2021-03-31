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


bool CPU::getInterruptMasterEnableFlag()
{
    return interruptMasterEnableFlag;
}


void CPU::setInterruptMasterEnableFlag(bool state)
{
    interruptMasterEnableFlag = state;
}


uint32_t CPU::execute(int32_t cyclesLeftToRun)
{
    uint8_t opcode;
    uint8_t opcodeInstructionTypeX;
    uint8_t opcodeInstructionTypeY;
    uint8_t opcodeInstructionTypeZ;
    uint8_t *opcode8BitRegister1;
    uint8_t *opcode8BitRegister2;
    uint16_t *opcode16BitRegister;
    uint8_t cbOpcode;
    uint8_t cbInstructionType;
    uint8_t cbBitNumber;
    uint8_t *cbRegister;

    //if (((registers.PC >= 0x8000) & (registers.PC < 0xC000)) || ((registers.PC > 0xE000) && (registers.PC < 0xFF80)) || (registers.PC == 0xFFFF))
    //    int j = 0;

    //memory->writeByte(0x100, 0xE8);
    //memory->writeByte(0x101, -20);

    while (cyclesLeftToRun > 0)
    {
        opcode = memory->readByte(registers.PC);
        clockCyclesExecuted = clockCyclesTable[opcode];

        if (registers.PC == 0xC2c6)
            int i = 0;

        if (stopped == false)
        {
            if (halted == false)
                registers.PC++;
            else if ((halted == true) && (interruptMasterEnableFlag == false) && ((ioPorts->getInterruptRequestFlags() & 0x1F) != 0x00) && ((memory->readByte(0xFFFF) & 0x1F) != 0x00))
            {
                halted = false; // This accounts for the HALT bug which stops the Program Counter from incrementing for one instruction
                                // if HALT is used with IME disabled while there are interrupts enabled in IE and/or pending. Otherwise, the system halts permanently.
            }
        }

        else
        {
            // TO DO: Handle stopped condition.
        }

        // Break down the opcode.
        opcodeInstructionTypeX = (opcode & 0b11000000) >> 6;
        opcodeInstructionTypeY = (opcode & 0b00111000) >> 3;
        opcodeInstructionTypeZ = opcode & 0b00000111;

        // Determine the first register that will be used in 8-bit instructions.
        switch (opcodeInstructionTypeY & 0b111)
        {
            // nullptr represents the address that register HL points to.
            case 0b000: opcode8BitRegister1 = &registers.B; break;
            case 0b001: opcode8BitRegister1 = &registers.C; break;
            case 0b010: opcode8BitRegister1 = &registers.D; break;
            case 0b011: opcode8BitRegister1 = &registers.E; break;
            case 0b100: opcode8BitRegister1 = &registers.H; break;
            case 0b101: opcode8BitRegister1 = &registers.L; break;
            case 0b110: opcode8BitRegister1 = nullptr; break;
            case 0b111: opcode8BitRegister1 = &registers.A; break;
        }

        // Determine the second register that will be used in 8-bit instructions.
        switch (opcodeInstructionTypeZ & 0b111)
        {
            // nullptr represents the address that register HL points to.
            case 0b000: opcode8BitRegister2 = &registers.B; break;
            case 0b001: opcode8BitRegister2 = &registers.C; break;
            case 0b010: opcode8BitRegister2 = &registers.D; break;
            case 0b011: opcode8BitRegister2 = &registers.E; break;
            case 0b100: opcode8BitRegister2 = &registers.H; break;
            case 0b101: opcode8BitRegister2 = &registers.L; break;
            case 0b110: opcode8BitRegister2 = nullptr; break;
            case 0b111: opcode8BitRegister2 = &registers.A; break;
        }

        // Determine the register that will be used in 16-bit instructions.
        switch (opcodeInstructionTypeY & 0b110)
        {
            // Determine register pointer.
            case 0b000: opcode16BitRegister = &registers.BC; break;
            case 0b010: opcode16BitRegister = &registers.DE; break;
            case 0b100: opcode16BitRegister = &registers.HL; break;
            case 0b110: opcode16BitRegister = &registers.SP; break;
        }

        switch (opcodeInstructionTypeX)
        {
            case 0b00:
            {
                switch (opcodeInstructionTypeZ)
                {
                    case 0b000: // Relative jumps and assorted opcodes.
                    {
                        switch (opcodeInstructionTypeY)
                        {
                            case 0b000: z80_nop(); break;
                            case 0b001: z80_ld_addr16_sp(); break;
                            case 0b010: z80_stop(); break;
                            case 0b011: z80_jr_offset(); break;
                            case 0b100: z80_jr_nz(); break;
                            case 0b101: z80_jr_z(); break;
                            case 0b110: z80_jr_nc(); break;
                            case 0b111: z80_jr_c(); break;
                        }
                    } break;


                    case 0b001: // 16-bit load immediate and reg HL add.
                    {
                        switch (opcodeInstructionTypeY & 0b001)
                        {
                            // Load 16-bit register with immediate.
                            case 0b000: z80_ld_reg16_dat16(opcode16BitRegister); break;
                            // Add HL, reg16.
                            case 0b001: z80_add_reghl_reg16(opcode16BitRegister); break;
                        }
                    } break;


                    case 0b010: // Load using 16-bit pointer.
                    {
                        switch (opcodeInstructionTypeY & 0b001)
                        {
                            // Load pointer address with register A.
                            case 0b000:
                            {
                                switch (opcodeInstructionTypeY & 0b110)
                                {
                                    case 0b000: z80_ld_reg16_addr16_reg8(&registers.BC, &registers.A); break;
                                    case 0b010: z80_ld_reg16_addr16_reg8(&registers.DE, &registers.A); break;
                                    case 0b100: z80_ldi_reghl_addr16_rega(); break;
                                    case 0b110: z80_ldd_reghl_addr16_rega(); break;
                                }
                            } break;


                            // Load register A with pointer address data.
                            case 0b001:
                            {
                                switch (opcodeInstructionTypeY & 0b110)
                                {
                                    case 0b000: z80_ld_rega_addr16_reg16(&registers.BC); break;
                                    case 0b010: z80_ld_rega_addr16_reg16(&registers.DE); break;
                                    case 0b100: z80_ldi_rega_reghl_addr16(); break;
                                    case 0b110: z80_ldd_rega_reghl_addr16(); break;
                                }
                            } break;
                        }
                    } break;


                    // Increment and decrement 16-bit register.
                    case 0b011:
                    {
                        switch (opcodeInstructionTypeY & 0b001)
                        {
                            case 0b000: z80_inc_reg16(opcode16BitRegister); break;
                            case 0b001: z80_dec_reg16(opcode16BitRegister); break;
                        }
                    } break;


                    // Increment 8-bit register.
                    case 0b100:
                    {
                        if (opcode8BitRegister1 != nullptr)
                            z80_inc_reg8(opcode8BitRegister1);
                        else
                            z80_inc_reghl_addr16();
                    } break;


                    // Decrement 8-bit register.
                    case 0b101:
                    {
                        if (opcode8BitRegister1 != nullptr)
                            z80_dec_reg8(opcode8BitRegister1);
                        else
                            z80_dec_reghl_addr16();
                    } break;


                    case 0b110:
                    {
                        if (opcode8BitRegister1 != nullptr)
                            z80_ld_reg8_dat8(opcode8BitRegister1);
                        else
                            z80_ld_reghl_addr16_dat8();
                    } break;


                    case 0b111:
                    {
                        switch (opcodeInstructionTypeY)
                        {
                            case 0b000: z80_rlca(); break;
                            case 0b001: z80_rrca(); break;
                            case 0b010: z80_rla(); break;
                            case 0b011: z80_rra(); break;
                            case 0b100: z80_daa(); break;
                            case 0b101: z80_cpl(); break;
                            case 0b110: z80_scf(); break;
                            case 0b111: z80_ccf(); break;
                        }
                    } break;
                }
            } break;


            // 8-bit register loading and HALT.
            case 0b01:
            {
                // Check if this is a HALT instruction.
                if ((opcodeInstructionTypeY == 0b110) && (opcodeInstructionTypeZ == 0b110))
                    z80_halt();
                else
                {
                    if (opcode8BitRegister1 != nullptr)
                    {
                        if (opcode8BitRegister2 != nullptr)
                            z80_ld_reg8_reg8(opcode8BitRegister1, opcode8BitRegister2);
                        else
                            z80_ld_reg8_reghl_addr16(opcode8BitRegister1);
                    }
                    else
                    {
                        z80_ld_reghl_addr16_reg8(opcode8BitRegister2);
                    }
                }
            } break;


            // ALU operations.
            case 0b10:
            {
                switch (opcodeInstructionTypeY)
                {
                    // ADD instruction.
                    case 0b000:
                    {
                        if (opcode8BitRegister2 != nullptr)
                            z80_add_rega_reg8(opcode8BitRegister2);
                        else
                            z80_add_rega_reghl_addr16();
                    } break;


                    // ADC instruction.
                    case 0b001:
                    {
                        if (opcode8BitRegister2 != nullptr)
                            z80_adc_rega_reg8(opcode8BitRegister2);
                        else
                            z80_adc_rega_reghl_addr16();
                    } break;


                    // SUB instruction.
                    case 0b010:
                    {
                        if (opcode8BitRegister2 != nullptr)
                            z80_sub_rega_reg8(opcode8BitRegister2);
                        else
                            z80_sub_rega_reghl_addr16();
                    } break;


                    // SBC instruction.
                    case 0b011:
                    {
                        if (opcode8BitRegister2 != nullptr)
                            z80_sbc_rega_reg8(opcode8BitRegister2);
                        else
                            z80_sbc_rega_reghl_addr16();
                    } break;


                    // AND instruction.
                    case 0b100:
                    {
                        if (opcode8BitRegister2 != nullptr)
                            z80_rega_and_reg8(opcode8BitRegister2);
                        else
                            z80_rega_and_reghl_addr16();
                    } break;


                    // XOR instruction.
                    case 0b101:
                    {
                        if (opcode8BitRegister2 != nullptr)
                            z80_rega_xor_reg8(opcode8BitRegister2);
                        else
                            z80_rega_xor_reghl_addr16();
                    } break;


                    // OR instruction.
                    case 0b110:
                    {
                        if (opcode8BitRegister2 != nullptr)
                            z80_rega_or_reg8(opcode8BitRegister2);
                        else
                            z80_rega_or_reghl_addr16();
                    } break;


                    // CP instruction.
                    case 0b111:
                    {
                        if (opcode8BitRegister2 != nullptr)
                            z80_rega_cp_reg8(opcode8BitRegister2);
                        else
                            z80_rega_cp_reghl_addr16();
                    } break;
                }
            } break;


            case 0b11:
            {
                switch (opcodeInstructionTypeZ)
                {
                    case 0b000:
                    {
                        switch (opcodeInstructionTypeY)
                        {
                            case 0b000: z80_ret_nz(); break;
                            case 0b001: z80_ret_z(); break;
                            case 0b010: z80_ret_nc(); break;
                            case 0b011: z80_ret_c(); break;
                            case 0b100: z80_ldh_addr8_rega(); break;
                            case 0b101: z80_add_sp_dat8(); break;
                            case 0b110: z80_ldh_rega_addr8(); break;
                            case 0b111: z80_ld_reghl_sp_add_dat8(); break;
                        }
                    } break;


                    case 0b001:
                    {
                        switch (opcodeInstructionTypeY & 0b001)
                        {
                            case 0b000:
                            {
                                switch (opcodeInstructionTypeY & 0b110)
                                {
                                    case 0b000: z80_pop_reg16(&registers.BC); break;
                                    case 0b010: z80_pop_reg16(&registers.DE); break;
                                    case 0b100: z80_pop_reg16(&registers.HL); break;
                                    case 0b110: z80_pop_reg16(&registers.AF); break;
                                }
                            } break;


                            case 0b001:
                            {
                                switch (opcodeInstructionTypeY & 0b110)
                                {
                                    case 0b000: z80_ret(); break;
                                    case 0b010: z80_reti(); break;
                                    case 0b100: z80_jp_reghl(); break;
                                    case 0b110: z80_ld_sp_reghl(); break;
                                }
                            } break;
                        }
                    } break;


                    case 0b010:
                    {
                        switch (opcodeInstructionTypeY & 0b111)
                        {
                            case 0b000: z80_jp_nz(); break;
                            case 0b001: z80_jp_z(); break;
                            case 0b010: z80_jp_nc(); break;
                            case 0b011: z80_jp_c(); break;
                            case 0b100: z80_ld_regc_port_rega(); break;
                            case 0b101: z80_ld_addr16_rega(); break;
                            case 0b110: z80_ld_rega_regc_port(); break;
                            case 0b111: z80_ld_rega_addr16(); break;
                        }
                    } break;


                    case 0b011:
                    {
                        switch (opcodeInstructionTypeY & 0b111)
                        {
                            case 0b000: z80_jp_a16(); break;
                            //case 0b001: These are CB prefix codes and will be handled in their own branch.
                            case 0b010: return 0; break;
                            case 0b011: return 0; break;
                            case 0b100: return 0; break;
                            case 0b101: return 0; break;
                            case 0b110: z80_ei(); break;
                            case 0b111: z80_di(); break;
                        }
                    } break;


                    case 0b100:
                    {
                        switch (opcodeInstructionTypeY & 0b111)
                        {
                            case 0b000: z80_call_nz(); break;
                            case 0b001: z80_call_z(); break;
                            case 0b010: z80_call_nc(); break;
                            case 0b011: z80_call_c(); break;
                            case 0b100: return 0; break;
                            case 0b101: return 0; break;
                            case 0b110: return 0; break;
                            case 0b111: return 0; break;
                        }
                    } break;


                    case 0b101:
                    {
                        switch (opcodeInstructionTypeY & 0b001)
                        {
                            case 0b000:
                            {
                                switch (opcodeInstructionTypeY & 0b110) {
                                    case 0b000: z80_push_reg16(&registers.BC); break;
                                    case 0b010: z80_push_reg16(&registers.DE); break;
                                    case 0b100: z80_push_reg16(&registers.HL); break;
                                    case 0b110: z80_push_reg16(&registers.AF); break;
                                }
                            } break;


                            case 0b001:
                            {
                                switch (opcodeInstructionTypeY & 0b110) {
                                    case 0b000: z80_call_a16(); break;
                                    case 0b010: return 0; break;
                                    case 0b100: return 0; break;
                                    case 0b110: return 0; break;
                                }
                            } break;
                        }
                    } break;


                    case 0b110:
                    {
                        switch (opcodeInstructionTypeY & 0b111)
                        {
                            case 0b000: z80_add_rega_dat8(); break;
                            case 0b001: z80_adc_rega_dat8(); break;
                            case 0b010: z80_sub_rega_dat8(); break;
                            case 0b011: z80_sbc_rega_dat8(); break;
                            case 0b100: z80_rega_and_dat8(); break;
                            case 0b101: z80_rega_xor_dat8(); break;
                            case 0b110: z80_rega_or_dat8(); break;
                            case 0b111: z80_rega_cp_dat8(); break;
                        }
                    } break;


                    case 0b111: z80_rst(opcodeInstructionTypeY << 3); break;
                }
            } break;
        }

        switch (opcode)
        {
            // Handle the extended CB bit manipulation instructions.
            case 0xCB:
            {
                cbOpcode = memory->readByte(registers.PC);
                clockCyclesExecuted += clockCyclesCBTable[cbOpcode];

                // Decode the opcode's instruction type, bits 7-6, where b00 = rotate/shift, b01 = test bit, b10 = reset bit, b11 = set bit
                cbInstructionType = cbOpcode & 0b11000000;

                // Decode the bit that will be operated on, bits 5-3, where b000 = bit 0, b001 = bit 1, b010 = bit 2, b011 = bit 3, b100 = bit 4, b101 = bit 5, b110 = bit 6, b111 = bit 7
                cbBitNumber = cbOpcode & 0b00111000;

                // Decode the register number to operate on, bits 2-0, where b000 = B, b001 = C, b010 = D, b011 = E, b100 = H, b101 = L, b110 = (HL), b111 = A
                switch (cbOpcode & 0b00000111)
                {
                case 0b00000000: cbRegister = &registers.B; break;
                case 0b00000001: cbRegister = &registers.C; break;
                case 0b00000010: cbRegister = &registers.D; break;
                case 0b00000011: cbRegister = &registers.E; break;
                case 0b00000100: cbRegister = &registers.H; break;
                case 0b00000101: cbRegister = &registers.L; break;
                case 0b00000110: cbRegister = nullptr; break; // nullptr represents (HL), which will be handled by its own set of functions.
                case 0b00000111: cbRegister = &registers.A; break;
                }

                if (cbRegister != nullptr)
                {
                    switch (cbInstructionType) {
                        // This is a rotate/shift instruction. In this case, the bit number serves as an index for the operation to perform where
                        //                          b000 = rlc, b001 = rrc, b010 = rl, b011 = rr, b100 = sla, b101 = sra, b110 = swap, b111 = srl
                        case 0b00000000:
                        {
                            switch (cbBitNumber)
                            {
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
                        case 0b01000000:
                        {
                            // Create a mask to test the bit with.
                            cbBitNumber >>= 3;
                            cbBitNumber = 1 << cbBitNumber;

                            z80_cb_test_reg8_bit(cbRegister, cbBitNumber);
                        } break;
                        case 0b10000000:
                        {
                            // Create a mask to reset the bit with.
                            cbBitNumber >>= 3;
                            cbBitNumber = 0xFF - (1 << cbBitNumber);

                            z80_cb_reset_reg8_bit(cbRegister, cbBitNumber);
                        } break;
                        case 0b11000000:
                        {
                            // Create a mask to set the bit with.
                            cbBitNumber >>= 3;
                            cbBitNumber = 1 << cbBitNumber;

                            z80_cb_set_reg8_bit(cbRegister, cbBitNumber);
                        } break;
                    }
                }
                // Handle (HL) cases.
                else
                {
                    switch (cbInstructionType)
                    {
                        case 0b00000000:
                        {
                            switch (cbBitNumber)
                            {
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
                        case 0b01000000:
                        {
                            // Create a mask to test the bit with.
                            cbBitNumber >>= 3;
                            cbBitNumber = 1 << cbBitNumber;

                            z80_cb_test_reghl_addr16_bit(cbBitNumber);
                        } break;
                        case 0b10000000:
                        {
                            // Create a mask to reset the bit with.
                            cbBitNumber >>= 3;
                            cbBitNumber = 0xFF - (1 << cbBitNumber);

                            z80_cb_reset_reghl_addr16_bit(cbBitNumber);
                        } break;
                        case 0b11000000:
                        {
                            // Create a mask to set the bit with.
                            cbBitNumber >>= 3;
                            cbBitNumber = 1 << cbBitNumber;

                            z80_cb_set_reghl_addr16_bit(cbBitNumber);
                        } break;
                    }
                }

                registers.PC++;
            } break;
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
