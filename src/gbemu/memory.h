#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>


class Memory
{
    public:
        Memory(uint8_t *romData, uint32_t romSizeInBytes);

        uint8_t read8bit(uint16_t address);
        uint8_t read16bit(uint16_t address);
        void write8bit(uint16_t address, uint8_t data);
        void write16bit(uint16_t address, uint16_t data);

    private:
        uint8_t *romBuffer;
        uint8_t *romBank0;
        uint8_t *romBank1;
        uint8_t *videoRam;
        uint8_t *externalRam;
        uint8_t *internalRamBank0;
        uint8_t *internalRamBank1;
        uint8_t *spriteAttributeTable;
        uint8_t *ioRegisters;
        uint8_t *highRam;
        uint8_t *interruptEnable;
};

#endif // MEMORY_H
