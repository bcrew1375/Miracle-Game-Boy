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
        void getBackgroundScanline();
        uint32_t backgroundPixelBuffer[160 * 144];
        uint32_t finalDisplayBuffer[160 * 144];
        uint8_t backGroundTileMap[32 * 32];
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
