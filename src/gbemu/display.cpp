#include "Display.h"
#include "IoPorts.h"
#include "bitmanipulation.h"

#include <iostream>
#include <memory>


Display::Display(std::shared_ptr<const uint8_t[]> videoRam,
                 std::shared_ptr<const uint8_t[]> spriteAttributeTable,
                 std::shared_ptr<IOPorts> ioPorts)
{
    this->videoRam = videoRam;
    this->spriteAttributeTable = spriteAttributeTable;
    this->ioPorts = ioPorts;

    backgroundTileMap = std::make_unique<uint8_t[]>(BACKGROUND_TILE_MAP_SIZE);
    backgroundWindowScanlineBuffer = std::make_unique<uint8_t[]>(BACKGROUND_WINDOW_SCANLINE_BUFFER_SIZE);
    finalizedScanlineBuffer = std::make_unique<uint8_t[]>(FINALIZED_SCANLINE_BUFFER_SIZE);
    spriteScanlineBuffer = std::make_unique<uint8_t[]>(SPRITE_SCANLINE_BUFFER_SIZE);
    tileLineBuffer = std::make_unique<uint8_t[]>(TILE_LINE_BUFFER_SIZE);

    finalDisplayBuffer = std::make_shared<uint32_t[]>(FRAME_BUFFER_SIZE);
    std::fill(finalDisplayBuffer.get(), finalDisplayBuffer.get() + FRAME_BUFFER_SIZE, 0xFFFFFFFF);
}


std::shared_ptr<uint32_t[]> Display::getFrameBuffer() const
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
            finalDisplayBuffer[x + (currentLcdYCoordinate * 160)] = rgbaPixelColors[finalizedScanlineBuffer[x]];
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

    tileLineBuffer[0] = paletteColors[((tileData & Bits::BIT_15_ON) >> 14) + ((tileData & Bits::BIT_7_ON) >> 7)];
    tileLineBuffer[1] = paletteColors[((tileData & Bits::BIT_14_ON) >> 13) + ((tileData & Bits::BIT_6_ON) >> 6)];
    tileLineBuffer[2] = paletteColors[((tileData & Bits::BIT_13_ON) >> 12) + ((tileData & Bits::BIT_5_ON) >> 5)];
    tileLineBuffer[3] = paletteColors[((tileData & Bits::BIT_12_ON) >> 11) + ((tileData & Bits::BIT_4_ON) >> 4)];
    tileLineBuffer[4] = paletteColors[((tileData & Bits::BIT_11_ON) >> 10) + ((tileData & Bits::BIT_3_ON) >> 3)];
    tileLineBuffer[5] = paletteColors[((tileData & Bits::BIT_10_ON) >>  9) + ((tileData & Bits::BIT_2_ON) >> 2)];
    tileLineBuffer[6] = paletteColors[((tileData & Bits::BIT_9_ON ) >>  8) + ((tileData & Bits::BIT_1_ON) >> 1)];
    tileLineBuffer[7] = paletteColors[((tileData & Bits::BIT_8_ON ) >>  7) + ((tileData & Bits::BIT_0_ON)     )];
}


void Display::getBackgroundTileMap()
{
    switch (ioPorts->getLcdControl() & 0x08) {
    case 0x00: std::copy(videoRam.get() + 0x1800, videoRam.get() + 0x1800 + 0x400, backgroundTileMap.get()); break;
    case 0x08: std::copy(videoRam.get() + 0x1C00, videoRam.get() + 0x1C00 + 0x400, backgroundTileMap.get()); break;
    }
}


// Collects one full line of tile bytes from the 32 x 32 map.
void Display::getBackgroundWindowScanline()
{
    uint16_t tileDataPointerBase;
    uint16_t windowTileMapOffset;

    uint8_t backgroundPalette = ioPorts->getBackgroundPalette();
    uint8_t currentLcdYCoordinate = ioPorts->getLcdYCoordinate();
    uint8_t scrollXOffset = ioPorts->getScrollX();
    uint8_t scrollYOffset = ioPorts->getScrollY();

    uint8_t tileDataOffsetX = scrollXOffset % TILE_WIDTH;
    uint8_t tileDataOffsetY = (currentLcdYCoordinate + scrollYOffset) % TILE_HEIGHT;
    uint8_t tileNumber;
    uint8_t tileNumberOffsetX = (uint8_t)(scrollXOffset / TILE_WIDTH);
    uint8_t tileNumberOffsetY = (uint8_t)((currentLcdYCoordinate + scrollYOffset) / TILE_HEIGHT) % BACKGROUND_TILE_MAP_HEIGHT;

    int16_t windowXOffset = ioPorts->getWindowX() - 7;
    int16_t windowYOffset = ioPorts->getWindowY();

    // Disable background and window for sprite testing.
    //ioPorts->setLcdControl(ioPorts->getLcdControl() & 0xFE);

    // Check the background and window are enabled before drawing.
    if (ioPorts->getLcdControl() & Bits::BIT_0_ON)
    {
        switch (ioPorts->getLcdControl() & Bits::BIT_4_ON)
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
            tileNumber = backgroundTileMap[((mapX + tileNumberOffsetX) % BACKGROUND_TILE_MAP_WIDTH) + (tileNumberOffsetY * BACKGROUND_TILE_MAP_HEIGHT)];

            if (tileDataPointerBase == 0x0000)
                convertTileData(videoRam[tileDataPointerBase + (tileNumber * TILE_BYTE_WIDTH) + (tileDataOffsetY * 2)] +
                               (videoRam[tileDataPointerBase + (tileNumber * TILE_BYTE_WIDTH) + (tileDataOffsetY * 2) + 1] << 8),
                                backgroundPalette, false);
            else
                convertTileData(videoRam[tileDataPointerBase + ((int8_t)(tileNumber) * TILE_BYTE_WIDTH) + (tileDataOffsetY * 2)] +
                               (videoRam[tileDataPointerBase + ((int8_t)(tileNumber) * TILE_BYTE_WIDTH) + (tileDataOffsetY * 2) + 1] << 8),
                                backgroundPalette, false);

            for (int x = 0; x < 8; x++)
            {
                // Only draw pixels that are in the viewport.
                if ((((x - tileDataOffsetX) + (mapX * 8)) >= 0) && ((x - tileDataOffsetX + (mapX * 8)) < 160))
                    finalizedScanlineBuffer[((x - tileDataOffsetX) + (mapX * 8))] = tileLineBuffer[x];
            }
        }

        if ((currentLcdYCoordinate == 0) && ((ioPorts->getLcdStatus() & (Bits::BIT_0_ON | Bits::BIT_1_ON)) != Bits::BIT_0_ON))
            windowLineCounter = 0;

        // Disable window for testing.
        //ioPorts->setLcdControl(ioPorts->getLcdControl() & 0xDF);

        // The window has an additional enable bit. Check it and that the window is in the viewport before drawing.
        if ((ioPorts->getLcdControl() & 0x20) && (currentLcdYCoordinate >= windowYOffset) && (windowXOffset < 160))
        {
            switch (ioPorts->getLcdControl() & Bits::BIT_6_ON)
            {
            case 0x00: windowTileMapOffset = 0x1800; break;
            case 0x40: windowTileMapOffset = 0x1C00; break;
            default: windowTileMapOffset = 0x1800; break;
            }


            tileNumberOffsetY = windowLineCounter / TILE_HEIGHT;
            tileDataOffsetY = windowLineCounter % TILE_HEIGHT;


            for (int16_t windowX = windowXOffset; windowX < 160; windowX += TILE_WIDTH)
            {
                tileNumber = videoRam[windowTileMapOffset + ((windowX - windowXOffset) / TILE_WIDTH) + (tileNumberOffsetY * BACKGROUND_TILE_MAP_HEIGHT)];

                if (tileDataPointerBase == 0x0000)
                    convertTileData(videoRam[tileDataPointerBase + (tileNumber * TILE_BYTE_WIDTH) + (tileDataOffsetY * 2)] +
                                   (videoRam[tileDataPointerBase + (tileNumber * TILE_BYTE_WIDTH) + (tileDataOffsetY * 2) + 1] << 8),
                                    backgroundPalette, false);
                else
                    convertTileData(videoRam[tileDataPointerBase + ((int8_t)(tileNumber) * TILE_BYTE_WIDTH) + (tileDataOffsetY * 2)] +
                                   (videoRam[tileDataPointerBase + ((int8_t)(tileNumber) * TILE_BYTE_WIDTH) + (tileDataOffsetY * 2) + 1] << 8),
                                    backgroundPalette, false);

                for (uint8_t x = 0; x < TILE_WIDTH; x++)
                {
                    // Only draw pixels that are in the viewport.
                    if (((x + windowX) < 160) && ((x + windowX) >= 0))
                        finalizedScanlineBuffer[x + windowX] = tileLineBuffer[x];
                }
            }

            windowLineCounter++;
        }
    }
    else
    {
        std::fill(finalizedScanlineBuffer.get(), finalizedScanlineBuffer.get() + 160, backgroundPalette & (Bits::BIT_0_ON | Bits::BIT_1_ON));
    }
}


void Display::getSpriteScanline()
{
    bool backgroundWindowPriority;
    bool spriteXFlip;
    bool spriteYFlip;

    int16_t spriteXPosition;
    int16_t spriteYPosition;
    int16_t sprites[10][4];

    uint8_t backgroundPaletteColorIndex0 = ioPorts->getBackgroundPalette() & (Bits::BIT_0_ON | Bits::BIT_1_ON); // Necessary for background/window prioritization.
    uint8_t currentLcdYCoordinate = ioPorts->getLcdYCoordinate();
    uint8_t spriteCount = 0;
    uint8_t spriteHeight = 8 << ((ioPorts->getLcdControl() & Bits::BIT_2_ON) >> 2); // Get the global sprite tile height bit. 0 = 8, 1 = 16.
    uint8_t spritePalette;
    uint8_t tileDataOffsetX;
    uint8_t tileDataOffsetY;
    uint8_t tileNumber;


    // Disable sprites for testing.
    //ioPorts->setLcdControl(ioPorts->getLcdControl() & 0xFD);

    // Only draw if sprites are enabled.
    if (ioPorts->getLcdControl() & Bits::BIT_1_ON)
    {
        for (int spriteNumber = 0; spriteNumber < 40; spriteNumber++)
        {
            spriteYPosition = spriteAttributeTable[spriteNumber * SPRITE_ATTRIBUTE_BYTE_WIDTH] - 16;

            if ((currentLcdYCoordinate >= spriteYPosition) && (currentLcdYCoordinate < (spriteYPosition + spriteHeight)))
            {
                // Gather all sprite attributes.
                sprites[spriteCount][0] = spriteYPosition;
                sprites[spriteCount][1] = spriteAttributeTable[(spriteNumber * SPRITE_ATTRIBUTE_BYTE_WIDTH) + 1];
                sprites[spriteCount][2] = spriteAttributeTable[(spriteNumber * SPRITE_ATTRIBUTE_BYTE_WIDTH) + 2];
                sprites[spriteCount][3] = spriteAttributeTable[(spriteNumber * SPRITE_ATTRIBUTE_BYTE_WIDTH) + 3];

                spriteCount++;
            }

            if (spriteCount == 10)
                break;
        }

        // Reverse indexing allows sprite earlier in the OAM to be drawn above later sprites.
        for (int spriteNumber = (spriteCount - 1); spriteNumber >= 0; spriteNumber--)
        {
            spriteYPosition = sprites[spriteNumber][0];
            spriteXPosition = sprites[spriteNumber][1];
            tileNumber = sprites[spriteNumber][2];
            if (spriteHeight == TILE_HEIGHT_TALL)
                tileNumber &= 0xFE;


            backgroundWindowPriority = sprites[spriteNumber][3] & Bits::BIT_7_ON;
            spriteYFlip = sprites[spriteNumber][3] & Bits::BIT_6_ON;
            spriteXFlip = sprites[spriteNumber][3] & Bits::BIT_5_ON;
            switch (sprites[spriteNumber][3] & Bits::BIT_4_ON) {
            case 0x00: spritePalette = ioPorts->getSpritePalette0(); break;
            case 0x10: spritePalette = ioPorts->getSpritePalette1(); break;
            default: spritePalette = 0x00; break;
            }

            if (spriteYFlip)
                tileDataOffsetY = (currentLcdYCoordinate - spriteYPosition) ^ (spriteHeight - 1); // If flipped, invert the data offset.
            else
                tileDataOffsetY = (currentLcdYCoordinate - spriteYPosition);

            convertTileData(videoRam[(tileNumber * TILE_BYTE_WIDTH) + (tileDataOffsetY * 2)] +
                           (videoRam[(tileNumber * TILE_BYTE_WIDTH) + (tileDataOffsetY * 2) + 1] << 8), spritePalette, true);

            tileDataOffsetX = 0;

            for (int16_t spriteX = (spriteXPosition - TILE_WIDTH); spriteX < spriteXPosition; spriteX++)
            {
                if ((spriteX >= 0) && (spriteX < 160))
                {
                    if (spriteXFlip)
                        tileDataOffsetX ^= 7;
                    if (tileLineBuffer[tileDataOffsetX] != 0xFF) // Color 0 isn't drawn for sprites.
                    {
                        if (backgroundWindowPriority == false)
                            finalizedScanlineBuffer[spriteX] = tileLineBuffer[tileDataOffsetX];
                        else
                        {
                            // If the background/window takes priority, only overwrite the pixel color associated with index 0.
                            if (finalizedScanlineBuffer[spriteX] == backgroundPaletteColorIndex0)
                                finalizedScanlineBuffer[spriteX] = tileLineBuffer[tileDataOffsetX];
                        }
                    }
                    if (spriteXFlip)
                        tileDataOffsetX ^= 7;
                }
                tileDataOffsetX++;
            }
        }
    }
}
