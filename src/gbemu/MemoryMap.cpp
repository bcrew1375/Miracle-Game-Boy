#include "MemoryMap.h"
#include "ioports.h"

#include <iostream>
#include <memory>
#include <string>


MemoryMap::MemoryMap(std::unique_ptr<const uint8_t[]> bootROM,
                     std::unique_ptr<const uint8_t[]> romData,
                     uint32_t romSizeInBytes,
                     std::shared_ptr<IOPorts> ioPorts)
{
    // Ensure the ROM buffer contains at least 32,768 bytes (the minimum to fill both banks).
    if (romSizeInBytes < MINIMUM_ROM_SIZE)
    {
        throw std::runtime_error("The loaded ROM file is " + std::to_string(romSizeInBytes) + \
                                 "A Game Boy ROM should be between " + std::to_string(MINIMUM_ROM_SIZE) + \
                                 " and " + std::to_string(MAXIMUM_ROM_SIZE));
    }

    this->ioPorts = ioPorts;
    this->bootROM = std::move(bootROM);

    spriteAttributeTable = std::make_unique<uint8_t[]>(SPRITE_ATTRIBUTE_TABLE_SIZE);
    videoRam = std::make_unique<uint8_t[]>(VIDEO_RAM_SIZE);

    romBank0 = std::make_unique<uint8_t[]>(ROM_BANK_SIZE);
    romBank1 = std::make_unique<uint8_t[]>(ROM_BANK_SIZE);
    videoRam = std::make_unique<uint8_t[]>(VIDEO_RAM_SIZE);
    internalRamBank0 = std::make_unique<uint8_t[]>(INTERNAL_RAM_BANK_SIZE);
    internalRamBank1 = std::make_unique<uint8_t[]>(INTERNAL_RAM_BANK_SIZE);
    spriteAttributeTable = std::make_unique<uint8_t[]>(SPRITE_ATTRIBUTE_TABLE_SIZE);
    highRam = std::make_unique<uint8_t[]>(HIGH_RAM_SIZE);

    std::fill(videoRam.get(), videoRam.get() + VIDEO_RAM_SIZE, UNDEFINED_VALUE);
    std::fill(internalRamBank0.get(), internalRamBank0.get() + INTERNAL_RAM_BANK_SIZE, UNDEFINED_VALUE);
    std::fill(internalRamBank1.get(), internalRamBank1.get() + INTERNAL_RAM_BANK_SIZE, UNDEFINED_VALUE);
    std::fill(spriteAttributeTable.get(), spriteAttributeTable.get() + SPRITE_ATTRIBUTE_TABLE_SIZE, UNDEFINED_VALUE);
    std::fill(highRam.get(), highRam.get() + HIGH_RAM_SIZE, UNDEFINED_VALUE);

    interruptEnableFlags = 0x00;

    // Load the first 32,768 bytes into the two 16 K ROM banks.

    // It's necessary to copy to and move a non-const array to the const romBank arrays, since make_unique technically initializes the array.
    std::unique_ptr<uint8_t[]> romDataBank0Subset = std::make_unique<uint8_t[]>(ROM_BANK_SIZE);
    std::copy(romData.get(), romData.get() + ROM_BANK_SIZE, romDataBank0Subset.get());
    romBank0 = std::move(romDataBank0Subset);

    externalHardwareType = romBank0[0x0147];

    // Determine if the cartridge uses extra hardware.
    if (externalHardwareType != 0x00)
    {
        memoryBankController = std::make_unique<MemoryBankController>(std::move(romData));
    }
    else
    {
        memoryBankController = nullptr;
        const auto romDataBank1Offset = romData.get() + ROM_BANK_SIZE;

        std::unique_ptr<uint8_t[]> romDataBank1Subset = std::make_unique<uint8_t[]>(ROM_BANK_SIZE);
        std::copy(romDataBank1Offset, romDataBank1Offset + ROM_BANK_SIZE, romDataBank1Subset.get());
        romBank1 = std::move(romDataBank1Subset);
    }
}


MemoryMap::~MemoryMap()
{
}


uint32_t MemoryMap::getSaveRamSize() const
{
    if (memoryBankController != nullptr)
        return memoryBankController->getRamBankSize();
    else
        return 0;
}


std::shared_ptr<const uint8_t[]> MemoryMap::getSaveRamArray() const
{
    if (memoryBankController != nullptr)
        return memoryBankController->getRamBankArray();
    else
        return nullptr;
}


std::shared_ptr<const uint8_t[]> MemoryMap::getSpriteAttributeTableArray() const
{
    return spriteAttributeTable;
}


std::shared_ptr<const uint8_t[]> MemoryMap::getVideoRamArray() const
{
    return videoRam;
}


uint8_t MemoryMap::readByte(uint16_t address) const
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
            return memoryBankController->readExternalRam(address);
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

void MemoryMap::setSaveRam(std::unique_ptr<const uint8_t[]> saveRam, uint32_t saveSize)
{
    memoryBankController->setRamBanks(std::move(saveRam), saveSize);
}


void MemoryMap::writeByte(uint16_t address, uint8_t data)
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
        case 0xFF46: std:for (int i = 0x00; i < 0xA0; i++)
                     {
                         spriteAttributeTable[i] = this->readByte((data << 8) + i);
                     }
                     ioPorts->setDmaTransfer(data, spriteAttributeTable.get());
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
