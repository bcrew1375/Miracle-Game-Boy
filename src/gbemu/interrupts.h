#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

#include "cpu.h"
#include "memory.h"


class Interrupts
{
    public:
        Interrupts(Memory *memory, CPU *cpu);

    private:
        Memory *memory;
        CPU *cpu;
};

#endif // INTERRUPTS_H
