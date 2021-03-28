#ifndef SYSTEM_H
#define SYSTEM_H

#include "cpu.h"
#include "memory.h"
#include "display.h"
#include "ioports.h"

#include <stdint.h>
#include <string>


class System
{
    public:
        System(uint8_t *bootROM, uint8_t *romData, uint32_t romSizeInBytes);
        ~System();

        double getRefreshRate();
        void executeCycles();
        bool getIsRunning();
        std::string getSystemError();
        uint32_t *getFrameBuffer();
        void handleInterrupts();
        void setControllerInputs(bool *buttonInputs);

    private:
        Memory *memory;
        CPU *cpu;
        Display *display;
        IOPorts *ioPorts;

        bool isRunning;
        std::string systemError;

        uint32_t cyclesPerFrame;
        uint32_t clockSpeed;
        double displayRefreshRate;

        uint32_t cyclesExecuted;
        uint8_t previousOpcode;
        uint16_t previousPC;
};

#endif // SYSTEM_H
