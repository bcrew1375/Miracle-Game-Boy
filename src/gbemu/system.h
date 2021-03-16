#ifndef SYSTEM_H
#define SYSTEM_H

#include "cpu.h"
#include "memory.h"
#include "display.h"
#include "interrupts.h"

#include <stdint.h>
#include <string>


class System
{
    public:
        System(uint8_t *romData, uint32_t romSizeInBytes);

        double getRefreshRate();
        void executeCycles();
        bool getIsRunning();
        std::string getSystemError();

    private:
        Memory *memory;
        CPU *cpu;
        Interrupts *interrupts;
        Display *display;

        bool isRunning;
        std::string systemError;

        uint32_t cyclesPerFrame;
        uint32_t clockSpeed;
        double displayRefreshRate;

        int32_t cyclesLeftToRun;
        int32_t cyclesBeforeExecution;
        uint8_t previousOpcode;
        uint16_t previousPC;
};

#endif // SYSTEM_H
