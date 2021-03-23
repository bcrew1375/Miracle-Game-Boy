#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#include "ioports.h"

class Display
{
    public:
        Display(uint8_t *videoRam, uint8_t *spriteAttributeTable, IOPorts *ioPorts);

        void updateDisplayOutput();
        void createScanline();
        uint32_t *getFrameBuffer();

    private:
        uint32_t *convertTileData(uint16_t tileData);
        void getBackgroundTileMap();
        void getBackgroundTileBytesLine();
        void convertBackgroundTileBytes();
        uint32_t backgroundConvertedPixelBuffer[256 * 144];
        uint32_t finalDisplayBuffer[160 * 144];
        uint8_t backgroundTileMap[32 * 32];
        uint16_t backgroundTileBytes[32 * 144]; // This will store the bytes for 32 8-pixel wide 2bpp tiles across 144 lines.
        const uint32_t rgbaPixelColors[4] = {
                                        0xFFFFFF00, // White
                                        0xAAAAAA00, // Light Gray
                                        0x55555500, // Dark Gray
                                        0x00000000  // Black
                                      };

        IOPorts *ioPorts;
        uint8_t *videoRam;
        uint8_t *spriteAttributeTable;
};

#endif // DISPLAY_H
