#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#include "memory.h"

class CPU
{
    public:
        CPU(Memory *memory);

        uint32_t execute();
        void resetCPU();

    private:
        Memory *memory;

        union registers {
            struct { uint16_t AF, BC, DE, HL, SP, PC; };
            struct { uint8_t F, A, C, B, E, D, L, H; };
            struct { bool nullbit0:1, nullbit1:1, nullbit2:1, nullbit3:1, flagC:1, flagH:1, flagN:1, flagZ:1; };
        } registers;
        /*uint16_t registerAF;
        uint16_t registerBC;
        uint16_t registerDE;
        uint16_t registerHL;
        uint16_t registerSP;
        uint16_t registerPC;*/
        uint8_t opcode;
        uint32_t clockCyclesExecuted;

        uint8_t *getRegisterA();
        uint8_t *getRegisterF();
        uint8_t *getRegisterB();
        uint8_t *getRegisterC();
        uint8_t *getRegisterD();
        uint8_t *getRegisterE();
        uint8_t *getRegisterH();
        uint8_t *getRegisterL();

        void setRegisterA(uint8_t data);
        void setRegisterF(uint8_t data);
        void setRegisterB(uint8_t data);
        void setRegisterC(uint8_t data);
        void setRegisterD(uint8_t data);
        void setRegisterE(uint8_t data);
        void setRegisterH(uint8_t data);
        void setRegisterL(uint8_t data);

        void z80_nop();
        void z80_ld_reg16_dat16(uint16_t *reg16);
        void z80_ld_reg16_addr_reg8(uint16_t *reg16, uint8_t *reg8);
        void z80_inc_reg16(uint16_t *reg16);
        void z80_inc_reg8(uint8_t *reg8);
        void z80_dec_reg8(uint8_t *reg8);
        void z80_ld_reg8_dat8(uint8_t *reg8);
        void z80_rlca();
        void z80_ld_addr_sp();
        void z80_add_reghl_reg16(uint16_t *reg16);
        void z80_ld_rega_addr_reg16(uint16_t *reg16);
        void z80_dec_reg16(uint16_t *reg16);
        void z80_rrca();
        void z80_stop();
        void z80_rla();
        void z80_jr_offset();
        void z80_rra();
        void z80_jr_nz();
        void z80_ldi_reghl_addr_rega();
        void z80_daa();
        void z80_jr_z();
        void z80_ldi_rega_reghl_addr();
        void z80_cpl();
        void z80_jr_nc();
        void z80_ldd_reghl_addr_rega();
        void z80_inc_reghl_addr();
        void z80_dec_reghl_addr();
        void z80_ld_reghl_addr_dat8();
        void z80_scf();
        void z80_ldd_rega_reghl_addr();
        void z80_jr_c();
        void z80_ccf();
        void z80_ld_reg8_reg8(uint8_t *reg8dest, uint8_t *reg8src);
        void z80_ld_reg8_reghl_addr(uint8_t *reg8);
        void z80_ld_reghl_addr_reg8(uint8_t *reg8);
        void z80_halt();
        void z80_add_rega_reg8(uint8_t *reg8);
        void z80_add_rega_reghl_addr();
        void z80_adc_rega_reg8(uint8_t *reg8);
        void z80_adc_rega_reghl_addr();
        void z80_sub_rega_reg8(uint8_t *reg8);
        void z80_sub_rega_reghl_addr();
        void z80_sbc_rega_reg8(uint8_t *reg8);
        void z80_sbc_rega_reghl_addr();
        void z80_jp_a16();
};

#endif // CPU_H
