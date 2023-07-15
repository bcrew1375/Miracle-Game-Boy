#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#include "ioports.h"
#include "memorybankcontroller.h"


class Memory
{
    public:
        Memory(uint8_t* bootROM, uint8_t* romData, uint32_t romSizeInBytes, IOPorts* ioPorts);
        ~Memory();

        uint32_t getSaveRamSize() const;
        uint8_t* getSaveRamPointer();
        uint8_t* getSpriteAttributeTablePointer();
        uint8_t* getVideoRamPointer();
        uint8_t readByte(uint16_t address) const;

        void setSaveRam(uint8_t *saveRam, uint32_t saveSize);
        void writeByte(uint16_t address, uint8_t data);

    private:
        IOPorts* ioPorts;
        MemoryBankController* memoryBankController;

        uint8_t* romData;
        uint8_t highRam[0x7F];
        uint8_t internalRamBank0[0x1000];
        uint8_t internalRamBank1[0x1000];
        uint8_t interruptEnableFlags;
        uint8_t externalHardwareType;
        uint8_t romBank0[0x4000];
        uint8_t romBank1[0x4000];
        uint8_t spriteAttributeTable[0xA0];
        uint8_t videoRam[0x2000];
};

#endif // MEMORY_H
