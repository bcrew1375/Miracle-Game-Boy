#ifndef MEMORY_H
#define MEMORY_H

#include <memory>
#include <stdint.h>

#include "ioports.h"
#include "memorybankcontroller.h"


class MemoryMap
{
    public:
        MemoryMap(std::unique_ptr<const uint8_t[]> bootROM,
                  std::unique_ptr<const uint8_t[]> romData,
                  uint32_t romSizeInBytes,
                  std::shared_ptr<IOPorts> ioPorts);
        ~MemoryMap();

        uint32_t getSaveRamSize() const;
        std::shared_ptr<const uint8_t[]> getSaveRamArray() const;
        std::shared_ptr<const uint8_t[]> getSpriteAttributeTableArray() const;
        std::shared_ptr<const uint8_t[]> getVideoRamArray() const;
        uint8_t readByte(uint16_t address) const;

        void setSaveRam(std::unique_ptr<const uint8_t[]> saveRam, uint32_t saveSize);
        void writeByte(uint16_t address, uint8_t data);

    private:
        static const uint32_t MINIMUM_ROM_SIZE           = 0x8000; // Minimum size of 32,768 bytes.
        static const uint32_t MAXIMUM_ROM_SIZE           = 0x200000; // Maximum size of 2,097,152 bytes.

        static const uint32_t ROM_BANK_SIZE              = 0x4000;
        static const uint32_t VIDEO_RAM_SIZE             = 0x2000;
        static const uint32_t INTERNAL_RAM_BANK_SIZE     = 0x1000;
        static const uint32_t SPRITE_ATTRIBUTE_TABLE_SIZE = 0xA0;
        static const uint32_t HIGH_RAM_SIZE              = 0x7F;

        static const uint32_t ROM_BANK_0_OFFSET             = 0x0000;
        static const uint32_t ROM_BANK_1_OFFSET             = 0x4000;
        static const uint32_t VIDEO_RAM_OFFSET             = 0x8000;
        static const uint32_t EXTERNAL_RAM_BANK_OFFSET      = 0xA000;
        static const uint32_t INTERNAL_RAM_BANK_0_OFFSET     = 0xC000;
        static const uint32_t INTERNAL_RAM_BANK_1_OFFSET     = 0xD000;
        static const uint32_t MIRROR_RAM_BANK_0_OFFSET       = 0xE000;
        static const uint32_t MIRROR_RAM_BANK_1_OFFSET       = 0xF000;
        static const uint32_t SPRITE_ATTRIBUTE_TABLE_OFFSET = 0xFE00;
        static const uint32_t UNUSABLE_MEMORY_SPACE_OFFSET  = 0xFEA0;
        static const uint32_t IO_PORTS_OFFSET              = 0xFF00;
        static const uint32_t HIGH_RAM_OFFSET              = 0xFF80;
        static const uint32_t INTERRUPT_ENABLE_FLAGS_OFFSET = 0xFFFF;

        static const uint8_t UNDEFINED_VALUE = 0xFF;


        uint8_t interruptEnableFlags;
        uint8_t externalHardwareType;

        std::shared_ptr<IOPorts> ioPorts;
        std::unique_ptr<MemoryBankController> memoryBankController;

        std::unique_ptr<uint8_t[]> highRam;
        std::unique_ptr<uint8_t[]> internalRamBank0;
        std::unique_ptr<uint8_t[]> internalRamBank1;
        std::unique_ptr<const uint8_t[]> bootROM;
        std::unique_ptr<const uint8_t[]> romBank0;
        std::unique_ptr<const uint8_t[]> romBank1;

        std::shared_ptr<uint8_t[]> spriteAttributeTable;
        std::shared_ptr<uint8_t[]> videoRam;
};

#endif // MEMORY_H
