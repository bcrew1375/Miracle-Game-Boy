#include <iostream>

#include "display.h"


Display::Display(uint8_t *videoRam, uint8_t *spriteAttributeTable, IOPorts *ioPorts)
{
    this->videoRam = videoRam;
    this->spriteAttributeTable = spriteAttributeTable;
    this->ioPorts = ioPorts;

    backGroundTileMap = new uint8_t[0x400];
    // Holds the RGBA converted pixels of the 256 x 256 background.
    backgroundPixelBuffer = new uint32_t[23040];

    finalDisplayBuffer = new uint32_t[23040];
    memset(finalDisplayBuffer, 0, 23040 * sizeof(uint32_t));
}


void Display::updateDisplayOutput()
{
    memcpy(finalDisplayBuffer, backgroundPixelBuffer, 23040 * sizeof(uint32_t));
}


uint32_t *Display::getFrameBuffer()
{
    return finalDisplayBuffer;
}


void Display::createScanline()
{
    getBackgroundTileMap();
    getBackgroundScanline();
}


// Convert a tile from 2-bit color pixels to RGBA format.
uint32_t * Display::convertTileData(uint16_t tileData)
{
    uint32_t *rgbaLine = new uint32_t[8];

    if ((tileData != 0) && (tileData != 0xFFFF) && (tileData != 0xFF00) && (tileData != 0x00FF))
        int i = 0;
    //tileData = 0xCCAA;

    // Each pixel color is determined by creating a 2-bit value formed by combining the same bit of each byte.
    rgbaLine[0] = rgbaPixelColors[((tileData & 0x8000) >> 14) + ((tileData & 0x0080) >> 7)];
    rgbaLine[1] = rgbaPixelColors[((tileData & 0x4000) >> 13) + ((tileData & 0x0040) >> 6)];
    rgbaLine[2] = rgbaPixelColors[((tileData & 0x2000) >> 12) + ((tileData & 0x0020) >> 5)];
    rgbaLine[3] = rgbaPixelColors[((tileData & 0x1000) >> 11) + ((tileData & 0x0010) >> 4)];
    rgbaLine[4] = rgbaPixelColors[((tileData & 0x0800) >> 10) + ((tileData & 0x0008) >> 3)];
    rgbaLine[5] = rgbaPixelColors[((tileData & 0x0400) >>  9) + ((tileData & 0x0004) >> 2)];
    rgbaLine[6] = rgbaPixelColors[((tileData & 0x0200) >>  8) + ((tileData & 0x0002) >> 1)];
    rgbaLine[7] = rgbaPixelColors[((tileData & 0x0100) >>  7) + ((tileData & 0x0001)     )];

    //uint32_t line = rgbaPixelColors[10];

    return rgbaLine;
}


void Display::getBackgroundTileMap()
{
    switch ((ioPorts->getLcdControl() & 0x08) >> 3) {
    case 0: memcpy(backGroundTileMap, &videoRam[0x1800], 0x400); break;
    case 1: memcpy(backGroundTileMap, &videoRam[0x1C00], 0x400); break;
    }
}

void Display::getBackgroundScanline()
{
    //uint16_t backgroundTileBytes[32];
    uint16_t backgroundTileBytes[20];
    uint32_t backgroundConvertedPixels[256];
    uint16_t tileDataPointerBase;
    uint8_t currentLcdYCoordinate;
    uint32_t *convertedTileLine;
    uint8_t scrollXOffset;
    uint8_t scrollYOffset;
    uint8_t tileNumber;
    uint8_t tileDataOffsetX;
    uint8_t tileDataOffsetY;

    convertedTileLine = new uint32_t[8];

    switch ((ioPorts->getLcdControl() & 0x10) >> 4) {
    case 0: tileDataPointerBase = 0x1000; break;
    case 1: tileDataPointerBase = 0x0000; break;
    default: tileDataPointerBase = 0x0000; break;
    }

    currentLcdYCoordinate = ioPorts->getLcdYCoordinate();

    scrollXOffset = ioPorts->getScrollX();
    scrollYOffset = ioPorts->getScrollY();

    //tileNumberToDraw = backgroundMap[(((screenCurrentY * 32) + ScrollYOffset / 8) & 32) +
                                     //(((ScrollXOffset + screenCurrentX) / 8) & 32

    // Since each tile has an 8 pixel height, calculate the current Y coordinate of the tiles to grab.
    //tileDataOffsetX = scrollXOffset % 8;
    //tileDataOffsetY = scrollYOffset % 8;

    // Collect one line of the full background tile bytes.
    for (int i = 0; i < 20; i++) {
        tileNumber = backGroundTileMap[i + ((currentLcdYCoordinate / 8) * 32)];

        if (tileDataPointerBase == 0x0000)
            backgroundTileBytes[i] = (uint16_t)(videoRam[tileDataPointerBase + (tileNumber * 16)] + (videoRam[(tileDataPointerBase + (tileNumber * 16)) + 1] << 8));
        else
            backgroundTileBytes[i] = (uint16_t)(videoRam[tileDataPointerBase + ((int8_t)(tileNumber) * 16)] + (videoRam[(tileDataPointerBase + ((int8_t)(tileNumber) * 16)) + 1] << 8));

        convertedTileLine = convertTileData(backgroundTileBytes[i]);
        memcpy(&backgroundConvertedPixels[i * 8], convertedTileLine, 8 * sizeof (uint32_t));
        //backgroundTileBytes[i] = (videoRam[(tileDataAreaOffset + (backGroundTileMap[i  * 32]) * 16) + tileDataOffset] << 8) + videoRam[(tileDataAreaOffset + (backGroundTileMap[i  * 32]) * 16) + tileDataOffset] + 1;
        //convertedTileLine = convertTileData(backgroundTileBytes[i]);
        //memcpy(&backgroundConvertedPixels[i * 8], convertedTileLine, 8 * sizeof(uint32_t));
    }

    memcpy(&backgroundPixelBuffer[160 * currentLcdYCoordinate], backgroundConvertedPixels, 160 * sizeof(uint32_t));
}
