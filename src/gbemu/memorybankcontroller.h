#ifndef MEMORYBANKCONTROLLER_H
#define MEMORYBANKCONTROLLER_H

#include <stdint.h>


class MemoryBankController
{
    public:
        MemoryBankController(uint8_t *romData);
        ~MemoryBankController();

        uint8_t readAddress(uint16_t address);
        void writeAddress(uint16_t address, uint8_t data);

    private:
        uint8_t *romData;
        uint8_t mbcType;
        uint8_t romBank1[0x4000];
        uint8_t romBankNumber;
        uint8_t ramBankSize;
        bool hasExternalRam;
        bool hasRamBanks;
        bool ramEnabled;
        uint8_t ramBank[8][0x2000];
};

#endif // MEMORYBANKCONTROLLER_H
