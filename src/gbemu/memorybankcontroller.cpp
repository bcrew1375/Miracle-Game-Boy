#include <iostream>

#include "memorybankcontroller.h"


MemoryBankController::MemoryBankController(uint8_t *romData)
{
    this->romData = romData;
    romBankNumber = 0x01;

    mbcType = romData[0x0147];
    ramBankSize = romData[0x0149];
    hasExternalRam = false;
    ramEnabled = false;
    ramBankSelected = 0;

    switch (mbcType)
    {
        case 0x02:
        case 0x03: hasExternalRam = true; break;
        default: hasExternalRam = false; break;
    }

    switch (ramBankSize)
    {
        case 0x02: hasRamBanks = false; break;
        default: hasRamBanks = true; break;
    }

    std::memcpy(romBank1, &this->romData[0x4000 * romBankNumber], 0x4000);
}


MemoryBankController::~MemoryBankController()
{
}


uint8_t MemoryBankController::readAddress(uint16_t address)
{
    if ((address >= 0x4000) && (address < 0x8000))
        return romBank1[address - 0x4000];
    else if ((address >= 0xA000) && (address < 0xC000))
    {
        if (ramEnabled == true)
            return ramBank[ramBankSelected][address - 0xA000];
        else
            return 0xFF;
    }
    else
        return 0xFF;
}


void MemoryBankController::writeAddress(uint16_t address, uint8_t data)
{
    if (address < 0x2000)
    {
        if (data & 0xA)
            ramEnabled = true;
        else if (data == 0x00)
            ramEnabled = false;
    }
    else if ((address >= 0x2000) && (address < 0x4000))
    {
        data &= 0x1F;
        if (data == 0x00)
            data = 0x01;

        romBankNumber &= 0xE0;
        romBankNumber |= data;

        std::memcpy(romBank1, &romData[romBankNumber * 0x4000], 0x4000);
    }
    else if ((address >= 0x4000) && (address < 0x6000))
    {
        if (hasRamBanks == true)
            ramBankSelected = data;
    }
    else if ((address >= 0xA000) && (address < 0xC000))
    {
        if (ramEnabled == true)
            ramBank[ramBankSelected][address - 0xA000] = data;
    }
}
