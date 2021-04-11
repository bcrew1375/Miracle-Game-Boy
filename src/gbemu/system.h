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
        System(uint8_t *bootROM, uint8_t *romData, uint32_t romSizeInBytes, uint8_t *saveData, uint32_t saveDataSize);
        ~System();

        bool getIsRunning();

        double getRefreshRate();

        std::string getSystemError();

        uint32_t *getFrameBuffer();
        uint32_t getSaveDataSize();

        uint8_t *getSaveData();

        void executeCycles();
        void setControllerInputs(bool *buttonInputs);

    private:
        CPU *cpu;
        Display *display;
        IOPorts *ioPorts;
        Memory *memory;

        bool isRunning;

        double displayRefreshRate;

        int32_t cyclesLeftToRun;
        int32_t cyclesPerFrame;

        std::string systemError;

        uint16_t previousPC;
        uint32_t clockSpeed;
        uint8_t previousOpcode;
};

#endif // SYSTEM_H
