#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#include "ioports.h"
#include "memory.h"

class CPU
{
    public:
        CPU(Memory *memory, IOPorts *ioPorts);

        uint32_t execute();
        void resetCPU();
        uint16_t getRegisterPC();

        void handleInterrupts();
        uint8_t getOpcode();
        bool getInterruptMasterEnableFlag();
        void setInterruptMasterEnableFlag(bool state);

    private:
        Memory *memory;
        IOPorts *ioPorts;

        union registers {
            struct { uint16_t AF, BC, DE, HL, SP, PC; };
            struct { uint8_t F, A, C, B, E, D, L, H, SPl, SPh, PCl, PCh; };
            struct { bool nullbit0:1, nullbit1:1, nullbit2:1, nullbit3:1, flagC:1, flagH:1, flagN:1, flagZ:1; };
        } registers;

        bool halted;
        bool stopped;
        bool interruptMasterEnableFlag;
        uint8_t opcode;
        uint32_t clockCyclesExecuted;
        int16_t lcdStatModeCycles;

        void z80_adc_rega_dat8();
        void z80_adc_rega_reg8(uint8_t *reg8);
        void z80_adc_rega_reghl_addr16();
        void z80_add_rega_dat8();
        void z80_add_rega_reg8(uint8_t *reg8);
        void z80_add_rega_reghl_addr16();
        void z80_add_reghl_reg16(uint16_t *reg16);
        void z80_add_sp_dat8();
        void z80_call();
        void z80_call_c();
        void z80_call_nc();
        void z80_call_nz();
        void z80_call_z();
        void z80_ccf();
        void z80_cpl();
        void z80_daa();
        void z80_dec_reg16(uint16_t *reg16);
        void z80_dec_reg8(uint8_t *reg8);
        void z80_dec_reghl_addr16();
        void z80_di();
        void z80_ei();
        void z80_halt();
        void z80_inc_reg16(uint16_t *reg16);
        void z80_inc_reg8(uint8_t *reg8);
        void z80_inc_reghl_addr16();
        void z80_jp_a16();
        void z80_jp_c();
        void z80_jp_nc();
        void z80_jp_nz();
        void z80_jp_reghl();
        void z80_jp_z();
        void z80_jr_c();
        void z80_jr_nc();
        void z80_jr_nz();
        void z80_jr_offset();
        void z80_jr_z();
        void z80_ld_addr16_rega();
        void z80_ld_addr16_sp();
        void z80_ld_reg16_addr16_reg8(uint16_t *reg16, uint8_t *reg8);
        void z80_ld_reg16_dat16(uint16_t *reg16);
        void z80_ld_reg8_dat8(uint8_t *reg8);
        void z80_ld_reg8_reg8(uint8_t *reg8dest, uint8_t *reg8src);
        void z80_ld_reg8_reghl_addr16(uint8_t *reg8);
        void z80_ld_rega_addr16();
        void z80_ld_rega_addr16_reg16(uint16_t *reg16);
        void z80_ld_rega_regc_port();
        void z80_ld_regc_port_rega();
        void z80_ld_reghl_addr16_dat8();
        void z80_ld_reghl_addr16_reg8(uint8_t *reg8);
        void z80_ld_reghl_sp_add_dat8();
        void z80_ld_sp_reghl();
        void z80_ldd_rega_reghl_addr16();
        void z80_ldd_reghl_addr16_rega();
        void z80_ldh_addr8_rega();
        void z80_ldh_rega_addr8();
        void z80_ldi_rega_reghl_addr16();
        void z80_ldi_reghl_addr16_rega();
        void z80_nop();
        void z80_pop_reg16(uint16_t *reg16);
        void z80_push_reg16(uint16_t *reg16);
        void z80_rega_and_dat8();
        void z80_rega_and_reg8(uint8_t *reg8);
        void z80_rega_and_reghl_addr16();
        void z80_rega_cp_dat8();
        void z80_rega_cp_reg8(uint8_t *reg8);
        void z80_rega_cp_reghl_addr16();
        void z80_rega_or_dat8();
        void z80_rega_or_reg8(uint8_t *reg8);
        void z80_rega_or_reghl_addr16();
        void z80_rega_xor_dat8();
        void z80_rega_xor_reg8(uint8_t *reg8);
        void z80_rega_xor_reghl_addr16();
        void z80_ret();
        void z80_ret_c();
        void z80_ret_nc();
        void z80_ret_nz();
        void z80_ret_z();
        void z80_reti();
        void z80_rla();
        void z80_rlca();
        void z80_rra();
        void z80_rrca();
        void z80_rst(uint8_t addr8);
        void z80_sbc_rega_dat8();
        void z80_sbc_rega_reg8(uint8_t *reg8);
        void z80_sbc_rega_reghl_addr16();
        void z80_scf();
        void z80_stop();
        void z80_sub_rega_dat8();
        void z80_sub_rega_reg8(uint8_t *reg8);
        void z80_sub_rega_reghl_addr16();


        void z80_cb_rl_reg8(uint8_t *reg8);
        void z80_cb_test_reg8_bit7(uint8_t *reg8);
};

#endif // CPU_H
