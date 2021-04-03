#include <iostream>

#include "memorybankcontroller.h"


MemoryBankController::MemoryBankController(uint8_t *romData)
{
    this->romData = romData;
    romBankNumber = 0x01;

    mbcType = romData[0x0147];

    switch (mbcType)
    {
        /*case 0x01: romBank1 = new uint8_t[0x4000]; break;
        case 0x02: romBank1 = new uint8_t[0x4000]; break;
        case 0x03: romBank1 = new uint8_t[0x4000]; break;*/
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
    else
        return 0xFF;
}


void MemoryBankController::writeAddress(uint16_t address, uint8_t data)
{
    if ((address >= 0x2000) && (address < 0x4000))
    {
        data &= 0x1F;
        if (data == 0x00)
            data = 0x01;

        romBankNumber &= 0xE0;
        romBankNumber |= data;

        std::memcpy(romBank1, &romData[romBankNumber * 0x4000], 0x4000);
    }
}
