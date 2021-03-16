#include <iostream>

#include "memory.h"


Memory::Memory(uint8_t *romData, uint32_t romSizeInBytes)
{
    // Ensure the ROM buffer contains at least 32,768 bytes (the minimum to fill both banks).
    if (romSizeInBytes < 0x8000)
        this->romData = new uint8_t[0x8000];
    else
        this->romData = new uint8_t[romSizeInBytes];

    // Create all necessary memory space buffers.
    romBank0 = new uint8_t[0x4000];
    romBank1 = new uint8_t[0x4000];
    videoRam = new uint8_t[0x2000];
    externalRam = new uint8_t[0x1000];
    internalRamBank0 = new uint8_t[0x1000];
    internalRamBank1 = new uint8_t[0x1000];
    spriteAttributeTable = new uint8_t[0xA0];
    ioRegisters = new uint8_t[0x80];
    highRam = new uint8_t[0x7F];
    interruptEnable = new uint8_t[0x01];

    std::memcpy(this->romData, romData, romSizeInBytes);

    // Load the first 32,768 bytes into the two 16 K ROM banks.
    std::memcpy(romBank0, &this->romData[0x0000], 0x4000);
    std::memcpy(romBank1, &this->romData[0x4000], 0x4000);
}


uint8_t Memory::readByte(uint16_t address)
{
    if (address >= 0x0000 && address < 0x4000) {
        return romBank0[address];
    }
    else if (address < 0x8000) {
        return romBank1[address - 0x4000];
    }
    else if (address < 0xA000) {
        return videoRam[address - 0x8000];
    }
    // External RAM should only be readable if the cartridge supports it. Otherwise, return an undefined value(0xFF).
    else if (address < 0xC000) {
        return externalRam[address - 0xA000];
    }
    else if (address < 0xD000) {
        return internalRamBank0[address - 0xC000];
    }
    else if (address < 0xE000) {
        return internalRamBank1[address - 0xD000];
    }
    else if (address < 0xFE00) {
        return internalRamBank0[address - 0xE000];
    }
    else if (address < 0xFEA0) {
        return spriteAttributeTable[address - 0xFE00];
    }
    // Unusable memory space. Return an undefined value(0xFF);
    else if (address < 0xFF00) {
        return 0xFF;
    }
    else if (address < 0xFF80) {
        return ioRegisters[address - 0xFF00];
    }
    else if (address < 0xFFFE) {
        return highRam[address - 0xFF80];
    }
    else if (address == 0xFFFF) {
        return interruptEnable[0];
    }
    else
        return 0xFF;
}


uint16_t Memory::read16bit(uint16_t address)
{
    return 0;
}


void Memory::writeByte(uint16_t address, uint8_t data)
{
    // Writing to ROM bank 1 has no effect but leave this here for completion's sake.
    if (address >= 0x0000 && address < 0x4000) {

    }

    // Depending on the cartridge type, writing here may replace ROM bank 1 for the bank indicated by the written value;
    else if (address < 0x8000) {

    }

    else if (address < 0xA000) {
        videoRam[address - 0x8000] = data;
    }

    // External RAM should only be writable if the cartridge supports it. Otherwise, do nothing.
    else if (address < 0xC000) {
        //externalRam[address - 0xA000] = data;
    }

    else if (address < 0xD000) {
        internalRamBank0[address - 0xC000] = data;
    }

    else if (address < 0xE000) {
        internalRamBank1[address - 0xD000] = data;
    }

    // This space will retain the values from 0xC000-0xDDFF regardless of any write operation.
    else if (address < 0xFE00) {
    }

    else if (address < 0xFEA0) {
        spriteAttributeTable[address - 0xFE00] = data;
    }

    // Unusable memory space. Do nothing;
    else if (address < 0xFF00) {
    }

    // Writing to this space will have various effects depending on the register written to.
    else if (address < 0xFF80) {
        ioRegisters[address - 0xFF00] = data;
    }

    else if (address < 0xFFFE) {
        highRam[address - 0xFF80] = data;
    }

    // The 3 most significant bits of the IME are unwrittable and are always high.
    else if (address == 0xFFFF) {
        interruptEnable[0] = (data & 0x1F) | 0b11100000;
    }
}


void Memory::write16bit(uint16_t address, uint16_t data)
{

}
