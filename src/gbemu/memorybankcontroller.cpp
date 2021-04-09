#include <iostream>

#include "memorybankcontroller.h"


MemoryBankController::MemoryBankController(uint8_t *romData)
{
    this->romData = romData;
    romBankSelected = 0x0001;

    advancedRomBankingMode = true;
    hasExternalRam = false;
    externalHardwareType = romData[0x147];
    ramBankSelected = 0x00;
    ramBankSize = romData[0x0149];
    ramEnabled = false;
    romSize = romData[0x148];

    switch (externalHardwareType)
    {
        case 0x01:
        case 0x02:
        case 0x03:
        {
            mbcType = 1;
            //hasExternalRam = true;
        } break;
        case 0x05:
        case 0x06:
        {
            mbcType = 2;
        } break;
        case 0x0F:
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        {
            mbcType = 3;
        } break;
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1E:
        {
            mbcType = 5;
        } break;
        default: hasExternalRam = false; break;
    }

    switch (ramBankSize)
    {
        case 0x00:
        case 0x01:
        case 0x02: hasRamBanks = false; break;

        case 0x03:
        {
            hasRamBanks = true;
            numberOfRamBanks = 4;
        } break;
        case 0x04:
        {
            hasRamBanks = true;
            numberOfRamBanks = 16;
        } break;
        case 0x05:
        {
            hasRamBanks = true;
            numberOfRamBanks = 8;
        } break;
        default: hasRamBanks = false; break;
    }

    std::memcpy(romBank1, &this->romData[0x4000 * romBankSelected], 0x4000);
}


MemoryBankController::~MemoryBankController()
{
}


uint8_t MemoryBankController::readExternalRam(uint16_t address)
{
    if (ramEnabled == true)
        return ramBank[ramBankSelected][address - 0xA000];
    else
        return 0xFF;
}


uint8_t MemoryBankController::readRomBank1(uint16_t address)
{
    return romBank1[address - 0x4000];
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
    // Make sure the register can only address the rom banks it has.

    data &= (2 << romSize) - 1;
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

    std::memcpy(romBank1, &romData[romBankSelected * 0x4000], 0x4000);
}


void MemoryBankController::writeHighRomBankRegister(uint8_t data)
{
    if (advancedRomBankingMode == false)
    {
        if (hasRamBanks == true)
            ramBankSelected = data;
    }
    else
    {
        romBankSelected = ((data & 0x03) << 5) | (romBankSelected & 0x1F);
    }
}


void MemoryBankController::writeBankingModeRegister(uint8_t data)
{
    advancedRomBankingMode = data & 0x01;
}


void MemoryBankController::writeExternalRam(uint16_t address, uint8_t data)
{
    if (ramEnabled == true)
        ramBank[ramBankSelected][address - 0xA000] = data;
}
