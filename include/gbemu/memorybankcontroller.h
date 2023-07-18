#ifndef MEMORYBANKCONTROLLER_H
#define MEMORYBANKCONTROLLER_H

#include <memory>
#include <stdint.h>

class MemoryBankController
{
    public:
        MemoryBankController(std::shared_ptr<uint8_t[]> romData);
        ~MemoryBankController();

        std::shared_ptr<uint8_t[]> getRamBankPointer();
        uint8_t getRamBankSize() const;
        uint8_t readExternalRam(uint16_t address) const;
        uint8_t readRomBank1(uint16_t address) const;

        void setRamBanks(std::shared_ptr<uint8_t[]> ramData, uint32_t ramSize);
        void writeRamEnableRegister(uint8_t data);
        void writeLowRomBankRegister(uint8_t data);
        void writeHighRomBankRegister(uint8_t data);
        void writeBankingModeRegister(uint8_t data);
        void writeExternalRam(uint16_t address, uint8_t data);

    private:
        bool hasBatteryBackup;
        bool hasExternalRam;
        bool advancedRomBankingMode;
        bool hasSwitchableRamBanks;
        bool ramEnabled;

        static const uint32_t MAX_RAM_BANK_SIZE = 0x40000;
        static const uint32_t ROM_BANK_SIZE = 0x4000;
        static const uint32_t RAM_BANK_SIZE = 0x2000;

        std::shared_ptr<uint8_t[]> romData;
        uint8_t externalHardwareTypeCode;
        uint8_t mbcType;
        uint8_t numberOfRamBanks;
        uint8_t ramBankSelected;
        uint8_t ramBankSizeCode;
        std::shared_ptr<uint8_t[]> ramBank;
        std::shared_ptr<uint8_t[]> romBank1;
        uint16_t romBankSelected;
        uint8_t romSizeCode;
};

#endif // MEMORYBANKCONTROLLER_H