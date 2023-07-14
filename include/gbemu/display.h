#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#include "ioports.h"

class Display
{
    public:
        Display(uint8_t *videoRam, uint8_t *spriteAttributeTable, IOPorts *ioPorts);

        uint32_t *getFrameBuffer();

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

        IOPorts *ioPorts;

        uint32_t finalDisplayBuffer[160 * 144];

        uint8_t *spriteAttributeTable;
        uint8_t *videoRam;
        uint8_t backgroundTileMap[32 * 32];
        uint8_t backgroundWindowScanlineBuffer[160];
        uint8_t finalizedScanline[160];
        uint8_t spriteScanlineBuffer[160];
        uint8_t tileLine[8];
        uint8_t windowLineCounter;
};

#endif // DISPLAY_H
