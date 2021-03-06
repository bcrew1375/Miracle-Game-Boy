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

    // Initialize all necessary memory space buffers.

    memset(romBank0, 0xFF, 0x4000);
    memset(romBank1, 0xFF, 0x4000);
    memset(videoRam, 0x00, 0x2000);
    //memset(externalRam, 0xFF, 0x1000);
    memset(internalRamBank0, 0xFF, 0x1000);
    memset(internalRamBank1, 0xFF, 0x1000);
    memset(spriteAttributeTable, 0xFF, 0xA0);
    memset(highRam, 0xFF, 0x7F);
    interruptEnableFlags = 0x00;

    std::memcpy(this->romData, romData, romSizeInBytes);

    // Load the first 32,768 bytes into the two 16 K ROM banks.
    std::memcpy(romBank0, &this->romData[0x0000], 0x4000);

    externalHardwareType = romBank0[0x0147];

    // Determine if the cartridge uses extra hardware.
    if (externalHardwareType != 0x00)
        memoryBankController = new MemoryBankController(this->romData);
    else
    {
        memoryBankController = nullptr;
        std::memcpy(romBank1, &this->romData[0x4000], 0x4000);
    }
}


Memory::~Memory()
{
    delete romData;

    if (memoryBankController != nullptr)
        delete memoryBankController;
}


uint32_t Memory::getSaveRamSize()
{
    if (memoryBankController != nullptr)
        return memoryBankController->getNumberOfRamBanks() * 8192;
    else
        return 0;
}


uint8_t *Memory::getSaveRamPointer()
{
    if (memoryBankController != nullptr)
        return memoryBankController->getRamBankPointer();
    else
        return nullptr;
}


uint8_t *Memory::getSpriteAttributeTablePointer()
{
    return spriteAttributeTable;
}


uint8_t *Memory::getVideoRamPointer()
{
    return videoRam;
}


uint8_t Memory::readByte(uint16_t address)
{
    if (address < 0x4000) {
        return romBank0[address];
    }
    else if (address < 0x8000) {
        if (externalHardwareType != 0x00)
            return memoryBankController->readRomBank1(address);
        else
            return romBank1[address - 0x4000];
    }
    else if (address < 0xA000) {
        return videoRam[address - 0x8000];
    }
    // External RAM should only be readable if the cartridge supports it. Otherwise, return an undefined value(0xFF).
    else if (address < 0xC000) {
        if (externalHardwareType != 0x00)
            memoryBankController->readExternalRam(address);
        else
            return 0xFF;
    }
    else if (address < 0xD000) {
        return internalRamBank0[address - 0xC000];
    }
    else if (address < 0xE000) {
        return internalRamBank1[address - 0xD000];
    }
    else if (address < 0xF000) {
        return internalRamBank0[address - 0xE000];
    }
    else if (address < 0xFE00) {
        return internalRamBank1[address - 0xF000];
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
        case 0xFF07: return ioPorts->getTimerControl(); break;
        case 0xFF0F: return ioPorts->getInterruptRequestFlags(); break;
        case 0xFF10: return ioPorts->getSoundChannel1Sweep(); break;
        case 0xFF11: return ioPorts->getSoundChannel1Length(); break;
        case 0xFF26: return ioPorts->getSoundOnOff(); break;
        case 0xFF40: return ioPorts->getLcdControl(); break;
        case 0xFF41: return ioPorts->getLcdStatus(); break;
        case 0xFF42: return ioPorts->getScrollY(); break;
        case 0xFF43: return ioPorts->getScrollX(); break;
        case 0xFF44: return ioPorts->getLcdYCoordinate(); break;
        case 0xFF45: return ioPorts->getLcdYCompare(); break;
        case 0xFF47: return ioPorts->getBackgroundPalette(); break;
        case 0xFF48: return ioPorts->getSpritePalette0(); break;
        case 0xFF49: return ioPorts->getSpritePalette1(); break;
        case 0xFF4A: return ioPorts->getWindowY(); break;
        case 0xFF4B: return ioPorts->getWindowX(); break;
        default: return 0xFF; break;
        }
    }
    else if (address < 0xFFFF) {
        return highRam[address - 0xFF80];
    }
    else if (address == 0xFFFF) {
        return interruptEnableFlags;
    }
    else
        return 0xFF;
}

void Memory::setSaveRam(uint8_t *saveRam, uint32_t saveSize)
{
    memoryBankController->setRamBanks(saveRam, saveSize);
}


void Memory::writeByte(uint16_t address, uint8_t data)
{
    if (address < 0x2000) {
        if (externalHardwareType != 0x00)
            memoryBankController->writeRamEnableRegister(data);
    }

    else if (address < 0x4000) {
        if (externalHardwareType != 0x00)
            memoryBankController->writeLowRomBankRegister(data);
    }

    else if (address < 0x6000) {
        if (externalHardwareType != 0x00)
            memoryBankController->writeHighRomBankRegister(data);
    }

    else if (address < 0x8000) {
        if (externalHardwareType != 0x00)
            memoryBankController->writeBankingModeRegister(data);
    }

    else if (address < 0x9800) {
        videoRam[address - 0x8000] = data;
    }

    else if (address < 0xA000) {
        videoRam[address - 0x8000] = data;
    }

    // External RAM should only be writable if the cartridge supports it. Otherwise, do nothing.
    else if (address < 0xC000) {
        if (externalHardwareType != 0x00)
            memoryBankController->writeExternalRam(address, data);
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
        case 0xFF04: ioPorts->setDivider(); break;
        case 0xFF05: ioPorts->setTimerCounter(data); break;
        case 0xFF06: ioPorts->setTimerModulo(data); break;
        case 0xFF07: ioPorts->setTimerControl(data); break;
        case 0xFF0F: ioPorts->setInterruptRequestFlags(data); break;
        case 0xFF10: ioPorts->setSoundChannel1Sweep(data); break;
        case 0xFF11: ioPorts->setSoundChannel1Length(data); break;
        case 0xFF26: ioPorts->setSoundOnOff(data); break;
        case 0xFF40: ioPorts->setLcdControl(data); break;
        case 0xFF41: ioPorts->setLcdStatus(data); break;
        case 0xFF42: ioPorts->setScrollY(data); break;
        case 0xFF43: ioPorts->setScrollX(data); break;
        case 0xFF44: ioPorts->setLcdYCoordinate(); break;
        case 0xFF45: ioPorts->setLcdYCompare(data); break;
        case 0xFF46: for (int i = 0x00; i < 0xA0; i++)
                     {
                         spriteAttributeTable[i] = this->readByte((data << 8) + i);
                     }
                     ioPorts->setDmaTransfer(data, spriteAttributeTable);
        break;
        case 0xFF47: ioPorts->setBackgroundPalette(data); break;
        case 0xFF48: ioPorts->setSpritePalette0(data); break;
        case 0xFF49: ioPorts->setSpritePalette1(data); break;
        case 0xFF4A: ioPorts->setWindowY(data); break;
        case 0xFF4B: ioPorts->setWindowX(data); break;
        default: return; break;
        }
    }

    else if (address < 0xFFFF) {
        highRam[address - 0xFF80] = data;

        // For debugging purposes
        if (highRam[0x0B] == 0xFF)
            int j = 0;
    }

    else if (address == 0xFFFF) {
        interruptEnableFlags = data;
    }
}
