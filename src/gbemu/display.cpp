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
    //convertBackgroundTileBytes();
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
    //getSpriteLine();
}


// Convert a tile line from 2-bit color pixels to RGBA format.
void Display::convertTileData(uint16_t tileData)
{
    uint8_t backgroundPalette = ioPorts->getBackgroundPalette();
    uint8_t backgroundColors[4] = { (uint8_t)(backgroundPalette & 0x03), (uint8_t)((backgroundPalette & 0x0C) >> 2),
                                    (uint8_t)((backgroundPalette & 0x30) >> 4), (uint8_t)((backgroundPalette & 0xC0) >> 6) };

    // Decode and convert the tile bytes from 2bpp to RGBA format.
    rgbaLine[0] = rgbaPixelColors[backgroundColors[((tileData & 0x8000) >> 14) + ((tileData & 0x0080) >> 7)]];
    rgbaLine[1] = rgbaPixelColors[backgroundColors[((tileData & 0x4000) >> 13) + ((tileData & 0x0040) >> 6)]];
    rgbaLine[2] = rgbaPixelColors[backgroundColors[((tileData & 0x2000) >> 12) + ((tileData & 0x0020) >> 5)]];
    rgbaLine[3] = rgbaPixelColors[backgroundColors[((tileData & 0x1000) >> 11) + ((tileData & 0x0010) >> 4)]];
    rgbaLine[4] = rgbaPixelColors[backgroundColors[((tileData & 0x0800) >> 10) + ((tileData & 0x0008) >> 3)]];
    rgbaLine[5] = rgbaPixelColors[backgroundColors[((tileData & 0x0400) >>  9) + ((tileData & 0x0004) >> 2)]];
    rgbaLine[6] = rgbaPixelColors[backgroundColors[((tileData & 0x0200) >>  8) + ((tileData & 0x0002) >> 1)]];
    rgbaLine[7] = rgbaPixelColors[backgroundColors[((tileData & 0x0100) >>  7) + ((tileData & 0x0001)     )]];
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
    uint8_t currentLcdYCoordinate = ioPorts->getLcdYCoordinate();

    uint16_t tileDataPointerBase;
    uint8_t tileNumber;

    uint8_t scrollXOffset = ioPorts->getScrollX();
    uint8_t scrollYOffset = ioPorts->getScrollY();

    uint8_t tileNumberOffsetX = (uint8_t)(scrollXOffset / 8);
    uint8_t tileNumberOffsetY = (uint8_t)((currentLcdYCoordinate + scrollYOffset) / 8) % 32;

    uint8_t tileDataOffsetX = scrollXOffset % 8;
    uint8_t tileDataOffsetY = (currentLcdYCoordinate + scrollYOffset) % 8;


    switch ((ioPorts->getLcdControl() & 0x10) >> 4)
    {
    case 0: tileDataPointerBase = 0x1000; break;
    case 1: tileDataPointerBase = 0x0000; break;
    default: tileDataPointerBase = 0x0000; break;
    }

    // Iterate through one horizontal line of the background map tile bytes.
    // 21 iterations are necessary to account for when two tiles are partially on screen.
    for (int mapX = 0; mapX < 21; mapX++)
    {
        // Modulation of the X and Y values is necessary for map wrapping.
        tileNumber = backgroundTileMap[((mapX + tileNumberOffsetX) % 32) + (tileNumberOffsetY * 32)];

        if (tileDataPointerBase == 0x0000)
            convertTileData(videoRam[tileDataPointerBase + (tileNumber * 16) + (tileDataOffsetY * 2)] +
                           (videoRam[tileDataPointerBase + (tileNumber * 16) + (tileDataOffsetY * 2) + 1] << 8));
        else
            convertTileData(videoRam[tileDataPointerBase + ((int8_t)(tileNumber) * 16) + (tileDataOffsetY * 2)] +
                           (videoRam[tileDataPointerBase + ((int8_t)(tileNumber) * 16) + (tileDataOffsetY * 2) + 1] << 8));

        for (int x = 0; x < 8; x++)
            // Only draw pixels that are in the viewport.
            if ((((x - tileDataOffsetX) + (mapX * 8)) >= 0) && ((x - tileDataOffsetX + (mapX * 8)) < 160))
                finalDisplayBuffer[((x - tileDataOffsetX) + (mapX * 8)) + (currentLcdYCoordinate * 160)] = rgbaLine[x];
    }
}


void Display::getSpriteLine()
{
    uint8_t currentLcdYCoordinate = ioPorts->getLcdYCoordinate();
    uint8_t spriteXPosition;
    uint8_t spriteYPosition;


    for (int spriteNumber = 0; spriteNumber < 40; spriteNumber++)
    {
        spriteYPosition = spriteAttributeTable[spriteNumber] - 16;
        if (spriteYPosition >= currentLcdYCoordinate) {

        }
    }
}
