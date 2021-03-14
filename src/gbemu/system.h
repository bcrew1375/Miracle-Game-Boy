#ifndef SYSTEM_H
#define SYSTEM_H

#include "cpu.h"
#include "memory.h"
#include "display.h"

#include <stdint.h>

class System
{
    public:
        System(uint8_t *romData, uint32_t romSizeInBytes);

    private:
        CPU *cpu;
        Memory *memory;
        Display *display;
};

#endif // SYSTEM_H
