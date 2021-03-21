#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#include "ioports.h"

class Memory
{
    public:
        Memory(uint8_t *bootROM, uint8_t *romData, uint32_t romSizeInBytes, IOPorts *ioPorts);

        uint8_t readByte(uint16_t address);
        uint16_t read16bit(uint16_t address);
        void writeByte(uint16_t address, uint8_t data);
        void write16bit(uint16_t address, uint16_t data);
        uint8_t *getVideoRamPointer();
        uint8_t *getSpriteAttributeTablePointer();
        uint8_t *getIoRegistersPointer();

    private:
        IOPorts *ioPorts;

        uint8_t *romData;
        uint8_t *romBank0;
        uint8_t *romBank1;
        uint8_t *videoRam;
        uint8_t *externalRam;
        uint8_t *internalRamBank0;
        uint8_t *internalRamBank1;
        uint8_t *spriteAttributeTable;
        uint8_t *highRam;
        uint8_t *interruptEnable;
};

#endif // MEMORY_H
