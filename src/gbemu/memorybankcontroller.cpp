#include "MemoryBankController.h"

#include <iostream>
#include <memory>


MemoryBankController::MemoryBankController(std::unique_ptr<const uint8_t[]> romData)
{
    this->romData = std::move(romData);

    romBank1 = std::make_unique<uint8_t[]>(ROM_BANK_SIZE);
    ramBank = std::make_shared<uint8_t[]>(MAX_RAM_BANK_SIZE);

    romBankSelected = 0x0001;

    externalHardwareTypeCode = this->romData[0x147];
    romSizeCode = this->romData[0x148];
    ramBankSizeCode = this->romData[0x0149];

    advancedRomBankingMode = false;
    hasBatteryBackup = false;
    hasExternalRam = false;
    hasSwitchableRamBanks = false;
    ramEnabled = false;

    numberOfRamBanks = 0;
    ramBankSelected = 0x00;

    switch (externalHardwareTypeCode)
    {
        case 0x01:
        case 0x02: mbcType = 1; break;
        case 0x03:
        {
            mbcType = 1;
            hasExternalRam = true;
            hasBatteryBackup = true;
        } break;
        case 0x05: mbcType = 2; break;
        case 0x06:
        {
            mbcType = 2;
            hasBatteryBackup = true;
        } break;
        case 0x0F:
        case 0x10:
        {
            mbcType = 3;
            hasExternalRam = true;
            hasBatteryBackup = true;
        } break;
        case 0x11:
        case 0x12: mbcType = 3; break;
        case 0x13:
        {
            mbcType = 3;
            hasExternalRam = true;
            hasBatteryBackup = true;
        } break;
        case 0x19:
        case 0x1A: mbcType = 5; break;
        case 0x1B:
        {
            mbcType = 5;
            hasExternalRam = true;
            hasBatteryBackup = true;
        } break;
        case 0x1C:
        case 0x1D: mbcType = 5; break;
        case 0x1E:
        {
            mbcType = 5;
            hasExternalRam = true;
            hasBatteryBackup = true;
        } break;
        default: hasExternalRam = false; break;
    }

    switch (ramBankSizeCode)
    {
        case 0x00:
        case 0x01: hasSwitchableRamBanks = false; break;
        case 0x02:
        {
            hasSwitchableRamBanks = false;
            numberOfRamBanks = 1;
        } break;
        case 0x03:
        {
            hasSwitchableRamBanks = true;
            numberOfRamBanks = 4;
        } break;
        case 0x04:
        {
            hasSwitchableRamBanks = true;
            numberOfRamBanks = 16;
        } break;
        case 0x05:
        {
            hasSwitchableRamBanks = true;
            numberOfRamBanks = 8;
        } break;
        default: hasSwitchableRamBanks = false; break;
    }

    // It's necessary to copy to and move a non-const array to the const romBank array, since make_unique technically initializes the array.
    const auto romDataBank1Offset = this->romData.get() + ROM_BANK_SIZE;

    std::unique_ptr<uint8_t[]> romDataBank1Subset = std::make_unique<uint8_t[]>(ROM_BANK_SIZE);
    std::copy(romDataBank1Offset, romDataBank1Offset + ROM_BANK_SIZE, romDataBank1Subset.get());
    romBank1 = std::move(romDataBank1Subset);
}


MemoryBankController::~MemoryBankController()
{
}


std::shared_ptr<const uint8_t[]> MemoryBankController::getRamBankArray() const
{
    if (hasBatteryBackup == true)
    {
        return ramBank;
    }
    else
    {
        return nullptr;
    }
}


const uint32_t MemoryBankController::getRamBankSize() const
{
    return numberOfRamBanks * RAM_BANK_SIZE;
}


const uint8_t MemoryBankController::readExternalRam(uint16_t address) const
{
    if ((hasExternalRam == true) && (ramEnabled == true))
        return ramBank.get()[(ramBankSelected * RAM_BANK_SIZE) + (address - 0xA000)];
    else
        return 0xFF;
}


const uint8_t MemoryBankController::readRomBank1(uint16_t address) const
{
    return romBank1.get()[address - ROM_BANK_SIZE];
}


void MemoryBankController::setRamBanks(std::unique_ptr<const uint8_t[]> ramData, uint32_t ramSize)
{
    std::copy(ramData.get(), ramData.get() + RAM_BANK_SIZE, ramBank.get());
}


void MemoryBankController::writeRamEnableRegister(uint8_t data)
{
    if (data & 0x0A)
        ramEnabled = true;
    else
        ramEnabled = false;
}


void MemoryBankController::writeLowRomBankRegister(uint8_t data)
{
    // Make sure the register can only address the ROM banks the ROM has.

    data &= (2 << romSizeCode) - 1;
    switch (mbcType)
    {
        case 1:
        {
            data &= 0x1F;
            if (data == 0x00)
                data = 0x01;
            romBankSelected &= 0x00E0;
        } break;
        case 2:
        {
            data &= 0x0F;
            if (data == 0x00)
                data = 0x01;
            romBankSelected &= 0x00F0;
        } break;
        case 3:
        {
            data &= 0x7F;
            if (data == 0x00)
                data = 0x01;
            romBankSelected &= 0x0080;
        } break;
        case 5:
        {
            // Only for clarification that MBC5 uses all 8 bits.
            data &= 0xFF;
            romBankSelected &= 0xFF00;
        } break;
    }

    romBankSelected |= data;
    auto romDataOffset = romData.get() + (romBankSelected * ROM_BANK_SIZE);
    std::copy(romDataOffset, romDataOffset + ROM_BANK_SIZE, romBank1.get());
}


void MemoryBankController::writeHighRomBankRegister(uint8_t data)
{
    switch (mbcType)
    {
        case 1:
        {
            data &= 0x03;

            if (advancedRomBankingMode == false)
            {
                romBankSelected = ((data & 0x03) << 5) | (romBankSelected & 0x1F);
                romBankSelected &= (2 << romSizeCode) - 1;
            }
            else
            {
                if (hasSwitchableRamBanks == true)
                    ramBankSelected = data;
            }
        } break;
        case 3:
        {
            data &= 0x03;

            if (hasSwitchableRamBanks == true)
                ramBankSelected = data;
        } break;
    }

}


void MemoryBankController::writeBankingModeRegister(uint8_t data)
{
    advancedRomBankingMode = data & 0x01;
}


void MemoryBankController::writeExternalRam(uint16_t address, uint8_t data)
{
    if ((hasExternalRam == true) && (ramEnabled == true))
        ramBank.get()[(ramBankSelected * RAM_BANK_SIZE) + (address - 0xA000)] = data;
}
