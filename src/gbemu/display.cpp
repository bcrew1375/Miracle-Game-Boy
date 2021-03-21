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
    memcpy(finalDisplayBuffer, backgroundPixelBuffer, 160 * 144 * sizeof(uint32_t));
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
    uint16_t backgroundTileBytes[20];
    uint32_t backgroundConvertedPixels[256];
    uint32_t *convertedTileLine;
    uint16_t tileDataPointerBase;
    uint8_t currentLcdYCoordinate;
    uint8_t scrollXOffset;
    uint8_t scrollYOffset;
    uint8_t tileNumber;
    uint8_t tileDataOffsetX;
    uint8_t tileDataOffsetY;
    uint8_t tileNumberOffsetX;
    uint8_t tileNumberOffsetY;


    switch ((ioPorts->getLcdControl() & 0x10) >> 4) {
    case 0: tileDataPointerBase = 0x1000; break;
    case 1: tileDataPointerBase = 0x0000; break;
    default: tileDataPointerBase = 0x0000; break;
    }

    currentLcdYCoordinate = ioPorts->getLcdYCoordinate();

    scrollXOffset = ioPorts->getScrollX();
    scrollYOffset = ioPorts->getScrollY();

    // Offset the tilenumber X position in the map by the SCX value. Divide by 8 since the SCX value is in pixels and each pixel is 8 pixels wide.
    tileNumberOffsetX = 0;//scrollXOffset / 8;
    // Offset the tilenumber Y position in the map by the SCY value + the current line being drawn. Divide by 8 since the SCX value is in pixels and each pixel is 8 pixels tall.
    tileNumberOffsetY = currentLcdYCoordinate / 8;//(currentLcdYCoordinate + scrollYOffset) / 8;

    tileDataOffsetX = 0;//scrollXOffset % 8;
    tileDataOffsetY = currentLcdYCoordinate % 8;//(currentLcdYCoordinate + scrollYOffset) % 8;

    // Collect one line of the full background tile bytes.
    for (int i = 0; i < 20; i++) {
        tileNumber = backGroundTileMap[i + (tileNumberOffsetY * 32) + tileNumberOffsetX];

        if (tileDataPointerBase == 0x0000)
            backgroundTileBytes[i] = videoRam[tileDataPointerBase + (tileNumber * 16) + (tileDataOffsetY * 2) + tileDataOffsetX] +
                                    (videoRam[tileDataPointerBase + (tileNumber * 16) + (tileDataOffsetY * 2) + tileDataOffsetX + 1] << 8);
        else
            backgroundTileBytes[i] = videoRam[tileDataPointerBase + ((int8_t)(tileNumber) * 16) + (tileDataOffsetY * 2) + tileDataOffsetX] +
                                    (videoRam[tileDataPointerBase + ((int8_t)(tileNumber) * 16) + (tileDataOffsetY * 2) + tileDataOffsetX + 1] << 8);

        //memcpy(convertedTileLine, convertTileData(backgroundTileBytes[i]), 8 * sizeof(uint32_t));
        //memcpy(&backgroundConvertedPixels[i * 8], convertedTileLine, 8 * sizeof (uint32_t));
        //backgroundTileBytes[i] = (videoRam[(tileDataAreaOffset + (backGroundTileMap[i  * 32]) * 16) + tileDataOffset] << 8) + videoRam[(tileDataAreaOffset + (backGroundTileMap[i  * 32]) * 16) + tileDataOffset] + 1;
        //convertedTileLine = convertTileData(backgroundTileBytes[i]);
        //memcpy(&backgroundConvertedPixels[i * 8], convertedTileLine, 8 * sizeof(uint32_t));
        convertedTileLine = convertTileData(backgroundTileBytes[i]);
        memcpy(&backgroundPixelBuffer[(i * 8) + (160 * currentLcdYCoordinate)], convertedTileLine, 160 * sizeof(uint32_t));
        delete [] convertedTileLine;
    }
}
