#ifndef SYSTEM_H
#define SYSTEM_H

#include <memory>
#include <stdint.h>
#include <string>

class CPU;
class Display;
class IOPorts;
class MemoryMap;


class System
{
    public:
        System(std::unique_ptr<const uint8_t[]> bootROM,
               std::unique_ptr<const uint8_t[]> romData,
               const uint32_t romSizeInBytes,
               std::unique_ptr<const uint8_t[]> saveData,
               const uint32_t saveDataSize);
        ~System();

        static constexpr uint32_t clockSpeed = 4194304;
        static constexpr double displayRefreshRate = 59.73;

        const bool getIsRunning() const;

        std::string getSystemError() const;

        std::shared_ptr<uint32_t[]> getFrameBuffer() const;
        uint32_t getSaveDataSize() const;

        std::shared_ptr<const uint8_t[]> getSaveData() const;

        void executeCycles();
        void setControllerInputs(bool *buttonInputs);

    private:
        std::unique_ptr<CPU> cpu;
        std::shared_ptr<Display> display;
        std::shared_ptr<IOPorts> ioPorts;
        std::shared_ptr<MemoryMap> memoryMap;

        static constexpr uint32_t cyclesPerFrame = static_cast<uint32_t>(clockSpeed / displayRefreshRate);

        bool isRunning;

        int32_t cyclesLeftToRun;

        std::string systemError;

        uint16_t previousPC;
        uint8_t previousOpcode;
};

#endif // SYSTEM_H
