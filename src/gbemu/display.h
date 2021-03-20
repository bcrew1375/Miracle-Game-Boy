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
        uint32_t *backgroundPixelBuffer;
        uint32_t *finalDisplayBuffer;
        const uint32_t rgbaPixelColors[4] = {
                                        0xFFFFFF00, // White
                                        0xAAAAAA00, // Light Gray
                                        0x55555500, // Dark Gray
                                        0x00000000  // Black
                                      };
        IOPorts *ioPorts;

        uint8_t *videoRam;
        uint8_t *spriteAttributeTable;

        uint8_t *backGroundTileMap;
        uint8_t *tileData;
};

#endif // DISPLAY_H
