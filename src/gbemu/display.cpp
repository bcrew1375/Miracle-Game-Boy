#include <iostream>

#include "display.h"


Display::Display(uint8_t *videoRam, uint8_t *spriteAttributeTable, IOPorts *ioPorts)
{
    this->videoRam = videoRam;
    this->spriteAttributeTable = spriteAttributeTable;
    this->ioPorts = ioPorts;

    memset(finalDisplayBuffer, 0xFF, 160 * 144 * sizeof(uint32_t));
}


void Display::updateDisplayOutput()
{
    convertBackgroundTileBytes();
    //memcpy(finalDisplayBuffer, backgroundPixelBuffer, 160 * 144 * sizeof(uint32_t));
}


uint32_t *Display::getFrameBuffer()
{
    return finalDisplayBuffer;
}


void Display::createScanline()
{
    getBackgroundTileMap();
    getBackgroundTileBytesLine();
}


// Convert a tile line from 2-bit color pixels to RGBA format.
uint32_t * Display::convertTileData(uint16_t tileData)
{
    uint32_t *rgbaLine = new uint32_t[8];

    // Pokemon window tile test
    /*switch (ioPorts->getLcdYCoordinate() % 8) {
    case 0: tileData = 0x00FF; break;
    case 1: tileData = 0xFF7E; break;
    case 2: tileData = 0x8185; break;
    case 3: tileData = 0x8389; break;
    case 4: tileData = 0x8593; break;
    case 5: tileData = 0x8BA5; break;
    case 6: tileData = 0x97C9; break;
    case 7: tileData = 0xFF7E; break;
    }*/

    // Each pixel color is determined by creating a 2-bit value formed by combining the same bit of each byte.

    //uint32_t line = rgbaPixelColors[10];

    return rgbaLine;
}


void Display::getBackgroundTileMap()
{
    switch (ioPorts->getLcdControl() & 0x08) {
    case 0: memcpy(backgroundTileMap, &videoRam[0x1800], 0x400); break;
    case 1: memcpy(backgroundTileMap, &videoRam[0x1C00], 0x400); break;
    }
}

// Collects one full line of tile bytes from the 32 x 32 map.
void Display::getBackgroundTileBytesLine()
{
    uint16_t tileDataPointerBase;
    uint8_t tileNumber;
    uint8_t currentLcdYCoordinate = ioPorts->getLcdYCoordinate();;
    uint8_t scrollYOffset = ioPorts->getScrollY();
    uint8_t tileNumberOffsetY = (currentLcdYCoordinate/* + scrollYOffset*/) / 8;
    uint8_t tileDataOffsetY = (currentLcdYCoordinate/* + scrollYOffset*/) % 8;


    switch ((ioPorts->getLcdControl() & 0x10) >> 4) {
    case 0: tileDataPointerBase = 0x1000; break;
    case 1: tileDataPointerBase = 0x0000; break;
    default: tileDataPointerBase = 0x0000; break;
    }

    // Collect one horizontal line of the full background map tile bytes.
    for (int x = 0; x < 32; x++) {
        tileNumber = backgroundTileMap[x + (tileNumberOffsetY * 32)];

        if (tileDataPointerBase == 0x0000)
            backgroundTileBytes[x + (currentLcdYCoordinate * 32)] = videoRam[tileDataPointerBase + (tileNumber * 16) + (tileDataOffsetY * 2)] +
                                                                   (videoRam[tileDataPointerBase + (tileNumber * 16) + (tileDataOffsetY * 2) + 1] << 8);
        else
            backgroundTileBytes[x + (currentLcdYCoordinate * 32)] = videoRam[tileDataPointerBase + ((int8_t)(tileNumber) * 16) + (tileDataOffsetY * 2)] +
                                                                   (videoRam[tileDataPointerBase + ((int8_t)(tileNumber) * 16) + (tileDataOffsetY * 2) + 1] << 8);
    }
}


void Display::convertBackgroundTileBytes()
{
    uint8_t scrollXOffset = ioPorts->getScrollX();
    for (int y = 0; y < 144; y++) {
        for (int x = 0; x < 32; x++) {
            // Decode and convert the tile bytess from 2bpp to RGBA format.
            backgroundConvertedPixelBuffer[((x * 8)    ) + (y * 160)] = rgbaPixelColors[((backgroundTileBytes[x + (y * 32)] & 0x8000) >> 14) + ((backgroundTileBytes[x + (y * 32)] & 0x0080) >> 7)];
            backgroundConvertedPixelBuffer[((x * 8) + 1) + (y * 160)] = rgbaPixelColors[((backgroundTileBytes[x + (y * 32)] & 0x4000) >> 13) + ((backgroundTileBytes[x + (y * 32)] & 0x0040) >> 6)];
            backgroundConvertedPixelBuffer[((x * 8) + 2) + (y * 160)] = rgbaPixelColors[((backgroundTileBytes[x + (y * 32)] & 0x2000) >> 12) + ((backgroundTileBytes[x + (y * 32)] & 0x0020) >> 5)];
            backgroundConvertedPixelBuffer[((x * 8) + 3) + (y * 160)] = rgbaPixelColors[((backgroundTileBytes[x + (y * 32)] & 0x1000) >> 11) + ((backgroundTileBytes[x + (y * 32)] & 0x0010) >> 4)];
            backgroundConvertedPixelBuffer[((x * 8) + 4) + (y * 160)] = rgbaPixelColors[((backgroundTileBytes[x + (y * 32)] & 0x0800) >> 10) + ((backgroundTileBytes[x + (y * 32)] & 0x0008) >> 3)];
            backgroundConvertedPixelBuffer[((x * 8) + 5) + (y * 160)] = rgbaPixelColors[((backgroundTileBytes[x + (y * 32)] & 0x0400) >>  9) + ((backgroundTileBytes[x + (y * 32)] & 0x0004) >> 2)];
            backgroundConvertedPixelBuffer[((x * 8) + 6) + (y * 160)] = rgbaPixelColors[((backgroundTileBytes[x + (y * 32)] & 0x0200) >>  8) + ((backgroundTileBytes[x + (y * 32)] & 0x0002) >> 1)];
            backgroundConvertedPixelBuffer[((x * 8) + 7) + (y * 160)] = rgbaPixelColors[((backgroundTileBytes[x + (y * 32)] & 0x0100) >>  7) + ((backgroundTileBytes[x + (y * 32)] & 0x0001)     )];
        }
        for (int x = 0; x < 160; x++)
            finalDisplayBuffer[x + (y * 160)] = backgroundConvertedPixelBuffer[((x/* + scrollXOffset*/) % 255) + (y * 160)];
    }
}
