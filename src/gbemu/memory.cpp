#include <iostream>

#include "memory.h"
#include "ioports.h"


Memory::Memory(uint8_t *bootROM, uint8_t *romData, uint32_t romSizeInBytes, IOPorts *ioPorts)
{
    // Ensure the ROM buffer contains at least 32,768 bytes (the minimum to fill both banks).
    if (romSizeInBytes < 0x8000)
        this->romData = new uint8_t[0x8000];
    else
        this->romData = new uint8_t[romSizeInBytes];

    this->ioPorts = ioPorts;

    // Create all necessary memory space buffers.
    romBank0 = new uint8_t[0x4000];
    romBank1 = new uint8_t[0x4000];
    videoRam = new uint8_t[0x2000];
    externalRam = new uint8_t[0x1000];
    internalRamBank0 = new uint8_t[0x1000];
    internalRamBank1 = new uint8_t[0x1000];
    spriteAttributeTable = new uint8_t[0xA0];
    highRam = new uint8_t[0x7F];

    memset(romBank0, 0xFF, 0x4000);
    memset(romBank1, 0xFF, 0x4000);
    memset(videoRam, 0xFF, 0x2000);
    memset(externalRam, 0xFF, 0x1000);
    memset(internalRamBank0, 0xFF, 0x1000);
    memset(internalRamBank1, 0xFF, 0x1000);
    memset(spriteAttributeTable, 0xFF, 0xA0);
    memset(highRam, 0xFF, 0x7F);
    interruptEnableFlags = 0x00;


    std::memcpy(this->romData, romData, romSizeInBytes);

    // Load the first 32,768 bytes into the two 16 K ROM banks.
    std::memcpy(romBank0, &this->romData[0x0000], 0x4000);
    //std::memcpy(romBank0, &bootROM[0x0000], 0x100);
    std::memcpy(romBank1, &this->romData[0x4000], 0x4000);
}


uint8_t Memory::readByte(uint16_t address)
{
    if (address < 0x4000) {
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
        switch (address) {
        case 0xFF00: return ioPorts->getController(); break;
        case 0xFF01: return ioPorts->getSerialTransferData(); break;
        case 0xFF02: return ioPorts->getSerialTransferControl(); break;
        case 0xFF04: return ioPorts->getDivider(); break;
        case 0xFF05: return ioPorts->getTimerCounter(); break;
        case 0xFF06: return ioPorts->getTimerModulo(); break;
        case 0xFF07: return ioPorts->getTimerControl();
        case 0xFF0F: return ioPorts->getInterruptRequestFlags(); break;
        case 0xFF40: return ioPorts->getLcdControl(); break;
        case 0xFF41: return ioPorts->getLcdStatus(); break;
        case 0xFF42: return ioPorts->getScrollY(); break;
        case 0xFF43: return ioPorts->getScrollX(); break;
        case 0xFF44: return ioPorts->getLcdYCoordinate(); break;
        case 0xFF45: return ioPorts->getLcdYCompare(); break;
        default: return 0; break;
        }
    }
    else if (address < 0xFFFE) {
        return highRam[address - 0xFF80];
    }
    else if (address == 0xFFFF) {
        return interruptEnableFlags;
    }
    else
        return 0xFF;
}


void Memory::writeByte(uint16_t address, uint8_t data)
{
    // Writing to ROM bank 1 has no effect but leave this here for completion's sake.
    if (address >= 0x0000 && address < 0x4000) {
        // Temporary. Writes should not allowed here.
        romBank0[address] = data;
    }

    // Depending on the cartridge type, writing here may replace ROM bank 1 for the bank indicated by the written value;
    else if (address < 0x8000) {

    }

    else if (address < 0x9800) {
        videoRam[address - 0x8000] = data;
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

    // Unusable memory space. Do nothing.
    else if (address < 0xFF00) {
    }

    // Writing to this space will have various effects depending on the register written to.
    else if (address < 0xFF80) {
        switch (address) {
        case 0xFF00: ioPorts->setController(data); break;
        case 0xFF01: ioPorts->setSerialTransferData(data); break;
        case 0xFF02: ioPorts->setSerialTransferControl(data); break;
        case 0xFF04: ioPorts->setDivider(data); break;
        case 0xFF05: ioPorts->setTimerCounter(data); break;
        case 0xFF06: ioPorts->setTimerModulo(data); break;
        case 0xFF07: ioPorts->setTimerControl(data); break;
        case 0xFF0F: ioPorts->setInterruptRequestFlags(data); break;
        case 0xFF40: ioPorts->setLcdControl(data); break;
        case 0xFF41: ioPorts->setLcdStatus(data); break;
        case 0xFF42: ioPorts->setScrollY(data); break;
        case 0xFF43: ioPorts->setScrollX(data); break;
        case 0xFF44: ioPorts->setLcdYCoordinate(); break;
        case 0xFF45: ioPorts->setLcdYCompare(data); break;
        default: return; break;
        }
    }

    else if (address < 0xFFFE) {
        highRam[address - 0xFF80] = data;
    }

    // The 3 most significant bits of the IME are unwrittable and are always high.
    else if (address == 0xFFFF) {
        interruptEnableFlags = (data & 0x1F) | 0xE0;
    }
}


void Memory::write16bit(uint16_t address, uint16_t data)
{

}


uint8_t *Memory::getSpriteAttributeTablePointer()
{
    return spriteAttributeTable;
}


uint8_t *Memory::getIoRegistersPointer()
{
    //return ioPortRegisters;
    return 0;
}


uint8_t *Memory::getVideoRamPointer()
{
    return videoRam;
}
