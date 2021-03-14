#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#include "memory.h"


class CPU
{
    public:
        CPU(Memory *memory);

        void resetCPU();

    private:
        Memory *memory;
        uint16_t registerAF;
        uint16_t registerBC;
        uint16_t registerDE;
        uint16_t registerHL;
        uint16_t registerSP;
        uint16_t registerPC;
};

#endif // CPU_H
