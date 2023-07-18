#ifndef SYSTEM_H
#define SYSTEM_H

#include "cpu.h"
#include "memory.h"
#include "display.h"
#include "ioports.h"

#include <memory>
#include <stdint.h>
#include <string>


class System
{
    public:
        System(std::unique_ptr<uint8_t[]> bootROM,
               std::shared_ptr<uint8_t[]> romData,
               uint32_t romSizeInBytes,
               std::shared_ptr<uint8_t[]> saveData,
               uint32_t saveDataSize);
        ~System();

        bool getIsRunning() const;

        double getRefreshRate() const;

        std::string getSystemError() const;

        std::shared_ptr<uint32_t[]> getFrameBuffer() const;
        uint32_t getSaveDataSize() const;

        std::shared_ptr<uint8_t[]> getSaveData() const;

        void executeCycles();
        void setControllerInputs(bool *buttonInputs);

    private:
        std::shared_ptr<CPU> cpu;
        std::shared_ptr<Display> display;
        std::shared_ptr<IOPorts> ioPorts;
        std::shared_ptr<MemoryMap> memoryMap;

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
