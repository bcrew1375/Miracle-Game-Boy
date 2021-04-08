#include <iostream>

#include "display.h"


Display::Display(uint8_t *videoRam, uint8_t *spriteAttributeTable, IOPorts *ioPorts)
{
    this->videoRam = videoRam;
    this->spriteAttributeTable = spriteAttributeTable;
    this->ioPorts = ioPorts;

    memset(finalDisplayBuffer, 0xFF, 160 * 144 * sizeof(uint32_t));
}


uint32_t *Display::getFrameBuffer()
{
    return finalDisplayBuffer;
}


void Display::createScanline()
{
    uint8_t currentLcdYCoordinate = ioPorts->getLcdYCoordinate();

    if (ioPorts->getLcdControl() & 0x80)
    {
        getBackgroundTileMap();
        getBackgroundWindowScanline();
        getSpriteScanline();

        for (int x = 0; x < 160; x++)
            finalDisplayBuffer[x + (currentLcdYCoordinate * 160)] = rgbaPixelColors[finalizedScanline[x]];
    }
    else
    {
        for (int x = 0; x < 160; x++)
            finalDisplayBuffer[x + (currentLcdYCoordinate * 160)] = rgbaPixelColors[0];
    }
}


// Convert a tile line's bytes to its palette color values.
void Display::convertTileData(uint16_t tileData, uint8_t palette, bool isSprite)
{
    uint8_t paletteColors[4] = { (uint8_t)(palette & 0x03), (uint8_t)((palette & 0x0C) >> 2),
                                 (uint8_t)((palette & 0x30) >> 4), (uint8_t)((palette & 0xC0) >> 6) };

    // This allows the sprite drawing routine to recognize color index 0
    // after substituting the palette values.
    if (isSprite == true)
        paletteColors[0] = 0xFF;

    tileLine[0] = paletteColors[((tileData & 0x8000) >> 14) + ((tileData & 0x0080) >> 7)];
    tileLine[1] = paletteColors[((tileData & 0x4000) >> 13) + ((tileData & 0x0040) >> 6)];
    tileLine[2] = paletteColors[((tileData & 0x2000) >> 12) + ((tileData & 0x0020) >> 5)];
    tileLine[3] = paletteColors[((tileData & 0x1000) >> 11) + ((tileData & 0x0010) >> 4)];
    tileLine[4] = paletteColors[((tileData & 0x0800) >> 10) + ((tileData & 0x0008) >> 3)];
    tileLine[5] = paletteColors[((tileData & 0x0400) >>  9) + ((tileData & 0x0004) >> 2)];
    tileLine[6] = paletteColors[((tileData & 0x0200) >>  8) + ((tileData & 0x0002) >> 1)];
    tileLine[7] = paletteColors[((tileData & 0x0100) >>  7) + ((tileData & 0x0001)     )];
}


void Display::getBackgroundTileMap()
{
    switch (ioPorts->getLcdControl() & 0x08) {
    case 0x00: memcpy(backgroundTileMap, &videoRam[0x1800], 0x400); break;
    case 0x08: memcpy(backgroundTileMap, &videoRam[0x1C00], 0x400); break;
    }
}


// Collects one full line of tile bytes from the 32 x 32 map.
void Display::getBackgroundWindowScanline()
{
    uint8_t currentLcdYCoordinate = ioPorts->getLcdYCoordinate();
    uint8_t backgroundPalette = ioPorts->getBackgroundPalette();

    uint16_t tileDataPointerBase;
    uint8_t tileNumber;

    uint8_t scrollXOffset = ioPorts->getScrollX();
    uint8_t scrollYOffset = ioPorts->getScrollY();

    uint8_t tileNumberOffsetX = (uint8_t)(scrollXOffset / 8);
    uint8_t tileNumberOffsetY = (uint8_t)((currentLcdYCoordinate + scrollYOffset) / 8) % 32;

    uint8_t tileDataOffsetX = scrollXOffset % 8;
    uint8_t tileDataOffsetY = (currentLcdYCoordinate + scrollYOffset) % 8;

    uint16_t windowTileMapOffset;
    uint8_t windowXOffset = ioPorts->getWindowX() - 7;
    uint8_t windowYOffset = ioPorts->getWindowY();

    // Disable background and window for sprite testing.
    //ioPorts->setLcdControl(ioPorts->getLcdControl() & 0xFE);

    // Check the background and window are enabled before drawing.
    if (ioPorts->getLcdControl() & 0x01)
    {
        switch (ioPorts->getLcdControl() & 0x10)
        {
        case 0x00: tileDataPointerBase = 0x1000; break;
        case 0x10: tileDataPointerBase = 0x0000; break;
        default: tileDataPointerBase = 0x0000; break;
        }

        // Iterate through one horizontal line of the background map tile bytes.
        // 21 iterations are necessary to account for when two tiles are partially on screen.
        for (uint8_t mapX = 0; mapX < 21; mapX++)
        {
            // Modulation of the X and Y values is necessary for map wrapping.
            tileNumber = backgroundTileMap[((mapX + tileNumberOffsetX) % 32) + (tileNumberOffsetY * 32)];

            if (tileDataPointerBase == 0x0000)
                convertTileData(videoRam[tileDataPointerBase + (tileNumber * 16) + (tileDataOffsetY * 2)] +
                               (videoRam[tileDataPointerBase + (tileNumber * 16) + (tileDataOffsetY * 2) + 1] << 8),
                                backgroundPalette, false);
            else
                convertTileData(videoRam[tileDataPointerBase + ((int8_t)(tileNumber) * 16) + (tileDataOffsetY * 2)] +
                               (videoRam[tileDataPointerBase + ((int8_t)(tileNumber) * 16) + (tileDataOffsetY * 2) + 1] << 8),
                                backgroundPalette, false);

            for (int x = 0; x < 8; x++)
            {
                // Only draw pixels that are in the viewport.
                if ((((x - tileDataOffsetX) + (mapX * 8)) >= 0) && ((x - tileDataOffsetX + (mapX * 8)) < 160))
                    finalizedScanline[((x - tileDataOffsetX) + (mapX * 8))] = tileLine[x];
            }
        }

        if (currentLcdYCoordinate == 0)
            windowLineCounter = 0;

        // The window has an additional enable bit. Check it before drawing the window.
        if ((ioPorts->getLcdControl() & 0x20) && ((currentLcdYCoordinate - windowYOffset) >= 0) && (windowXOffset < 160))
        {
            switch (ioPorts->getLcdControl() & 0x40)
            {
            case 0x00: windowTileMapOffset = 0x1800; break;
            case 0x40: windowTileMapOffset = 0x1C00; break;
            default: windowTileMapOffset = 0x1800; break;
            }


            tileNumberOffsetY = windowLineCounter / 8;
            tileDataOffsetY = windowLineCounter % 8;


            for (uint8_t windowX = windowXOffset; windowX < 160; windowX += 8)
            {
                tileNumber = videoRam[windowTileMapOffset + ((windowX - windowXOffset) / 8) + (tileNumberOffsetY * 32)];

                if (tileDataPointerBase == 0x0000)
                    convertTileData(videoRam[tileDataPointerBase + (tileNumber * 16) + (tileDataOffsetY * 2)] +
                                   (videoRam[tileDataPointerBase + (tileNumber * 16) + (tileDataOffsetY * 2) + 1] << 8),
                                    backgroundPalette, false);
                else
                    convertTileData(videoRam[tileDataPointerBase + ((int8_t)(tileNumber) * 16) + (tileDataOffsetY * 2)] +
                                   (videoRam[tileDataPointerBase + ((int8_t)(tileNumber) * 16) + (tileDataOffsetY * 2) + 1] << 8),
                                    backgroundPalette, false);

                for (uint8_t x = 0; x < 8; x++)
                {
                    // Only draw pixels that are in the viewport.
                    if ((x + windowX) < 160)
                        finalizedScanline[x + windowX] = tileLine[x];
                }
            }

            windowLineCounter++;
        }
    }
    else
    {
        memset(finalizedScanline, backgroundPalette & 0x03, 160);
    }
}


void Display::getSpriteScanline()
{
    uint8_t currentLcdYCoordinate = ioPorts->getLcdYCoordinate();
    uint8_t sprites[10][4];
    uint8_t spriteXPosition;
    int16_t spriteYPosition;
    uint8_t spriteHeight = 8 << ((ioPorts->getLcdControl() & 0x04) >> 2); // Get the global sprite tile height bit. 0 = 8, 1 = 16.
    uint8_t spritePalette;
    uint8_t spriteCount = 0;
    uint8_t backgroundPaletteColorIndex0 = ioPorts->getBackgroundPalette() & 0x03; // Necessary for background/window prioritization.
    bool backgroundWindowPriority;
    bool spriteXFlip;
    bool spriteYFlip;

    uint8_t tileNumber;

    uint8_t tileDataOffsetX;
    uint8_t tileDataOffsetY;

    // Only draw if sprites are enabled.
    if (ioPorts->getLcdControl() & 0x02)
    {
        for (int spriteNumber = 0; spriteNumber < 40; spriteNumber++)
        {
            spriteYPosition = spriteAttributeTable[spriteNumber * 4] - 16;

            if ((currentLcdYCoordinate >= spriteYPosition) && (currentLcdYCoordinate < (spriteYPosition + spriteHeight)))
            {
                // Gather all sprite attributes.
                sprites[spriteCount][0] = spriteYPosition;
                sprites[spriteCount][1] = spriteAttributeTable[(spriteNumber * 4) + 1];
                sprites[spriteCount][2] = spriteAttributeTable[(spriteNumber * 4) + 2];
                sprites[spriteCount][3] = spriteAttributeTable[(spriteNumber * 4) + 3];

                spriteCount++;
            }

            if (spriteCount == 10)
                break;
        }

        for (int spriteNumber = 0; spriteNumber < spriteCount; spriteNumber++)
        {
            spriteYPosition = sprites[spriteNumber][0];
            spriteXPosition = sprites[spriteNumber][1];
            tileNumber = sprites[spriteNumber][2];
            if (spriteHeight == 16)
                tileNumber &= 0xFE;


            backgroundWindowPriority = sprites[spriteNumber][3] & 0x80;
            spriteYFlip = sprites[spriteNumber][3] & 0x40;
            spriteXFlip = sprites[spriteNumber][3] & 0x20;
            switch (sprites[spriteNumber][3] & 0x10) {
            case 0x00: spritePalette = ioPorts->getSpritePalette0(); break;
            case 0x10: spritePalette = ioPorts->getSpritePalette1(); break;
            default: spritePalette = 0x00; break;
            }

            if (spriteYFlip)
                tileDataOffsetY = (currentLcdYCoordinate - spriteYPosition) ^ (spriteHeight - 1); // If flipped, invert the data offset.
            else
                tileDataOffsetY = (currentLcdYCoordinate - spriteYPosition);

            convertTileData(videoRam[(tileNumber * 16) + (tileDataOffsetY * 2)] +
                           (videoRam[(tileNumber * 16) + (tileDataOffsetY * 2) + 1] << 8), spritePalette, true);

            tileDataOffsetX = 0;

            for (int16_t spriteX = (spriteXPosition - 8); spriteX < spriteXPosition; spriteX++)
            {
                if ((spriteX >= 0) && (spriteX < 160))
                {
                    if (spriteXFlip)
                        tileDataOffsetX ^= 7;
                    if (tileLine[tileDataOffsetX] != 0xFF) // Color 0 isn't drawn for sprites.
                    {
                        if (backgroundWindowPriority == false)
                            finalizedScanline[spriteX] = tileLine[tileDataOffsetX];
                        else
                        {
                            // If the background/window takes priority, only overwrite the pixel color associated with index 0.
                            if (finalizedScanline[spriteX] == backgroundPaletteColorIndex0)
                                finalizedScanline[spriteX] = tileLine[tileDataOffsetX];
                        }
                    }
                    //else
                    //    finalizedScanline[spriteX] = rgbaPixelColors[3];
                    if (spriteXFlip)
                        tileDataOffsetX ^= 7;
                }
                tileDataOffsetX++;
            }
        }
    }
}
