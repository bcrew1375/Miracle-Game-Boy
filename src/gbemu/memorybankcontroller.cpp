#include <iostream>

#include "memorybankcontroller.h"


MemoryBankController::MemoryBankController(uint8_t *romData)
{
    this->romData = romData;
    romBankSelected = 0x0001;

    advancedRomBankingMode = false;
    hasBatteryBackup = false;
    hasExternalRam = false;
    externalHardwareTypeCode = romData[0x147];
    numberOfRamBanks = 0;
    ramBankSelected = 0x00;
    ramBankSizeCode = romData[0x0149];
    ramEnabled = false;
    romSizeCode = romData[0x148];

    memset(ramBank, 0xFF, 0x40000);

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

    romBank1 = &this->romData[0x4000 * romBankSelected];
}


MemoryBankController::~MemoryBankController()
{
}


uint8_t *MemoryBankController::getRamBankPointer()
{
    if (hasBatteryBackup == true)
        return ramBank;
    else
        return nullptr;
}


uint8_t MemoryBankController::getNumberOfRamBanks()
{
    return numberOfRamBanks;
}


uint8_t MemoryBankController::readExternalRam(uint16_t address)
{
    if ((hasExternalRam == true) && (ramEnabled == true))
        return ramBank[(ramBankSelected * 0x2000) + (address - 0xA000)];
    else
        return 0xFF;
}


uint8_t MemoryBankController::readRomBank1(uint16_t address)
{
    return romBank1[address - 0x4000];
}


void MemoryBankController::setRamBanks(uint8_t *ramData, uint32_t ramSize)
{
    memcpy(ramBank, ramData, ramSize);
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

    romBank1 = &romData[romBankSelected * 0x4000];
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
        ramBank[(ramBankSelected * 0x2000) + (address - 0xA000)] = data;
}
