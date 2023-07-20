#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <memory>

#include "ioports.h"

class Display
{
    public:
        Display(std::shared_ptr<const uint8_t[]> videoRam,
                std::shared_ptr<const uint8_t[]> spriteAttributeTable,
                std::shared_ptr<IOPorts> ioPorts);

        std::shared_ptr<uint32_t[]> getFrameBuffer() const;

        void createScanline();

    private:
        void convertTileData(uint16_t tileData, uint8_t palette, bool isSprite);
        void getBackgroundTileMap();
        void getBackgroundWindowScanline();
        void getSpriteScanline();

        const uint32_t rgbaPixelColors[4] = {
                                        0xFFFFFF00, // White
                                        0xAAAAAA00, // Light Gray
                                        0x55555500, // Dark Gray
                                        0x00000000  // Black
                                      };

        static const uint16_t FRAME_BUFFER_X_SIZE = 160;
        static const uint16_t FRAME_BUFFER_Y_SIZE = 144;
        static const uint32_t FRAME_BUFFER_SIZE = (FRAME_BUFFER_X_SIZE * FRAME_BUFFER_Y_SIZE);

        static const uint32_t BACKGROUND_TILE_MAP_SIZE = 32 * 32;
        static const uint32_t BACKGROUND_WINDOW_SCANLINE_BUFFER_SIZE = 160;
        static const uint32_t FINALIZED_SCANLINE_BUFFER_SIZE = 160;
        static const uint32_t SPRITE_SCANLINE_BUFFER_SIZE = 160;
        static const uint32_t TILE_LINE_BUFFER_SIZE = 8;

        std::shared_ptr<uint32_t[]> finalDisplayBuffer;

        std::shared_ptr<IOPorts> ioPorts;

        std::shared_ptr<const uint8_t[]> spriteAttributeTable;
        std::shared_ptr<const uint8_t[]> videoRam;
        
        std::unique_ptr<uint8_t[]> backgroundTileMap;
        std::unique_ptr<uint8_t[]> backgroundWindowScanlineBuffer;
        std::unique_ptr<uint8_t[]> finalizedScanlineBuffer;
        std::unique_ptr<uint8_t[]> spriteScanlineBuffer;
        std::unique_ptr<uint8_t[]> tileLineBuffer;
        uint8_t windowLineCounter;
};

#endif // DISPLAY_H
