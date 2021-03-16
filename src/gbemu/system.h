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

        double getRefreshRate();
        void executeCycles();
        //void runSystem(bool isRunning);
        bool isRunning();

    private:
        CPU *cpu;
        Memory *memory;
        Display *display;

        bool systemRunning;

        uint32_t cyclesPerFrame;
        uint32_t clockSpeed;
        double displayRefreshRate;

        int32_t cyclesLeftToRun;
};

#endif // SYSTEM_H
