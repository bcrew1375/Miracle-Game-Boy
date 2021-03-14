#ifndef INSTRUCTIONSET_H
#define INSTRUCTIONSET_H

#include <map>


class InstructionSet
{
    public:
        InstructionSet();

    private:
        void z80_nop();
        void z80_ld_reg16_d16();
};

#endif // INSTRUCTIONSET_H
