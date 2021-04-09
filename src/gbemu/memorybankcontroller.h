#ifndef MEMORYBANKCONTROLLER_H
#define MEMORYBANKCONTROLLER_H

#include <stdint.h>


class MemoryBankController
{
    public:
        MemoryBankController(uint8_t *romData);
        ~MemoryBankController();

        uint8_t readExternalRam(uint16_t address);

        void writeRamEnableRegister(uint8_t data);
        void writeLowRomBankRegister(uint8_t data);
        void writeHighRomBankRegister(uint8_t data);
        void writeBankingModeRegister(uint8_t data);
        void writeExternalRam(uint16_t address, uint8_t data);

    private:
        bool hasExternalRam;
        bool advancedRomBankingMode;
        bool hasRamBanks;
        bool ramEnabled;

        uint8_t *romData;
        uint8_t externalHardwareType;
        uint8_t mbcType;
        uint8_t ramBankSelected;
        uint8_t ramBankSize;
        uint8_t ramBank[8][0x2000];
        uint8_t romBank1[0x4000];
        uint16_t romBankSelected;
        uint8_t romSize;
};

#endif // MEMORYBANKCONTROLLER_H
