#include <iostream>

#include "ioports.h"


IOPorts::IOPorts()
{
    // All values assume bootrom has just finished executing.
    backgroundPalette = 0xFC;
    controller = 0xCF;
    dmaTransfer = 0x00;
    interruptRequestFlags = 0xE1;
    timerControl = 0xF8;
    timerCounter = 0x00;
    timerModulo = 0x00;
    lcdControl = 0x91;
    lcdStatus = 0x82;
    lcdStatModeCycles = 80;
    lcdYCoordinate = 0x00;
    lcdYCompare = 0x00;
    scrollY = 0;
    scrollX = 0;
    serialTransferControl = 0x7E;
    serialTransferData = 0x00;
    soundChannel1Sweep = 0x80;
    spritePalette0 = 0x00;
    spritePalette1 = 0x00;

    internalCounter = 0;
    hBlankBeginFlag = false;
}


bool IOPorts::getHBlankBeginFlag()
{
    if (hBlankBeginFlag == true) {
        hBlankBeginFlag = false;
        return true;
    }
    else
        return false;
}


uint8_t IOPorts::getBackgroundPalette()
{
    return backgroundPalette;
}


uint8_t IOPorts::getController()
{
    return controller;
}


uint8_t IOPorts::getDivider()
{
    return (internalCounter & 0xFF00) >> 8;
}


uint8_t IOPorts::getDmaTransfer()
{
    return dmaTransfer;
}


uint8_t IOPorts::getInterruptRequestFlags()
{
    return interruptRequestFlags;
}


uint8_t IOPorts::getLcdControl()
{
    return lcdControl;
}


uint8_t IOPorts::getLcdStatus()
{
    return lcdStatus;
}


uint8_t IOPorts::getLcdYCompare()
{
    return lcdYCompare;
}


uint8_t IOPorts::getLcdYCoordinate()
{
    return lcdYCoordinate;
}


uint8_t IOPorts::getScrollX()
{
    return scrollX;
}


uint8_t IOPorts::getScrollY()
{
    return scrollY;
}


uint8_t IOPorts::getSerialTransferControl()
{
    return serialTransferControl;
}


uint8_t IOPorts::getSerialTransferData()
{
    return serialTransferData;
}


uint8_t IOPorts::getSpritePalette0()
{
    return spritePalette0;
}


uint8_t IOPorts::getSpritePalette1()
{
    return spritePalette1;
}


uint8_t IOPorts::getTimerControl()
{
    return timerControl;
}


uint8_t IOPorts::getTimerCounter()
{
    return timerCounter;
}


uint8_t IOPorts::getTimerModulo()
{
    return timerModulo;
}


void IOPorts::setBackgroundPalette(uint8_t data)
{
    backgroundPalette = data;
}


void IOPorts::setController(uint8_t data)
{
    // Only bits 4-5 are writeable. Bits 0-3 are set hi/lo depending on controller inputs. Bits 6-7 are unusable.
    controller = 0xCF | (data & 0b00110000);

    if (!(controller & 0x10))
    {
        controller &= 0xF0;

        controller |= buttonInputs[0] << 3; // Down
        controller |= buttonInputs[1] << 2; // Up
        controller |= buttonInputs[2] << 1; // Left
        controller |= buttonInputs[3];      // Right

        controller ^= 0x0F;
    }
    else if (!(controller & 0x20))
    {
        controller &= 0xF0;

        controller |= buttonInputs[4] << 3; // Start
        controller |= buttonInputs[5] << 2; // Select
        controller |= buttonInputs[6] << 1; // B
        controller |= buttonInputs[7];      // A

        controller ^= 0x0F;
    }
}


void IOPorts::setControllerInputs(bool *buttonInputs)
{
    memcpy(&this->buttonInputs, buttonInputs, 8);
}


void IOPorts::setDivider(uint8_t data)
{
    internalCounter = 0;
}


void IOPorts::setDmaTransfer(uint8_t data, uint8_t *spriteAttributeTable)
{
    dmaTransfer = data;
    memcpy(this->spriteAttributeTable, spriteAttributeTable, 0xA0);
}


void IOPorts::setInterruptRequestFlags(uint8_t data)
{
    interruptRequestFlags = 0xE0 | data;
}


void IOPorts::setLcdControl(uint8_t data)
{
    lcdControl = data;

    if (!(lcdControl & 0x80)) {
        lcdStatus &= 0xFC;
        lcdYCoordinate = 0;
        lcdStatModeCycles = 80;
    }
}

void IOPorts::setLcdStatus(uint8_t data)
{
    // Make sure the current mode isn't overwritten.
    lcdStatus |= (data & 0xFC);
}


void IOPorts::setLcdYCompare(uint8_t data)
{
    lcdYCompare = data;
}


void IOPorts::setLcdYCoordinate()
{
    lcdYCoordinate = 0;
}


void IOPorts::setScrollX(uint8_t data)
{
    scrollX = data;
}


void IOPorts::setScrollY(uint8_t data)
{
    scrollY = data;
}


void IOPorts::setSerialTransferControl(uint8_t data)
{
    serialTransferControl = data;
}


void IOPorts::setSerialTransferData(uint8_t data)
{
    serialTransferData = data;
}


void IOPorts::setSpritePalette0(uint8_t data)
{
    spritePalette0 = data;
}


void IOPorts::setSpritePalette1(uint8_t data)
{
    spritePalette1 = data;
}


void IOPorts::setTimerControl(uint8_t data)
{
    timerControl = 0xF8 | data;
}


void IOPorts::setTimerCounter(uint8_t data)
{
    timerCounter = data;
}


void IOPorts::setTimerModulo(uint8_t data)
{
    timerModulo = data;
}


void IOPorts::updateLcdStatMode(uint16_t cyclesExecuted)
{
    uint8_t lcdStatMode = lcdStatus & 0x03;

    lcdStatModeCycles -= cyclesExecuted;
    if (lcdStatModeCycles <= 0)
    {
        if (lcdYCoordinate < 144)
        {
            switch (lcdStatMode)
            {
            case 0: lcdStatMode = 2; lcdStatModeCycles += 80; break;
            case 2: lcdStatMode = 3; lcdStatModeCycles += 168; break; // Variable between 168 and 291 depending on sprite count. Assume 168 for now.
            case 3: lcdStatMode = 0; lcdStatModeCycles += 208; break; // Variable between 85 and 208 depending on time taken for mode 3.
            }

            if (lcdStatMode == 0x00)
            {
                // Request H-Blank interrupt if enabled.
                if (lcdStatus & 0x08)
                    interruptRequestFlags |= 0x02;
                hBlankBeginFlag = true;
            }

            if (lcdStatMode == 0x02)
                lcdYCoordinate++;

            if (lcdYCoordinate == 144)
            {
                // V-Blank begins.
                lcdStatMode = 0x01;
                lcdStatModeCycles += 376; // Account for the 80 cycles added from switching to mode 2 by not adding them here.
                interruptRequestFlags |= 0x01;
                if (lcdStatus & 0x10)
                    interruptRequestFlags |= 0x02;
            }
        }

        else if (lcdYCoordinate >= 144)
        {
            lcdYCoordinate++;
            if (lcdYCoordinate == 154)
            {
                lcdYCoordinate = 0;
                lcdStatMode = 0x02;
                lcdStatModeCycles += 80;
            }
            else
                lcdStatModeCycles += 456;
        }
    }


    if (lcdYCoordinate == lcdYCompare) {
        lcdStatus |= 0x04;
        if (lcdStatus & 0x40)
            interruptRequestFlags |= 0x02;
    }
    else
        lcdStatus &= 0xFB;

    // Clear the 3 least significant bits and write the new status.
    lcdStatus &= 0xFC;
    lcdStatus |= lcdStatMode;
}


void IOPorts::updateRegisters(uint16_t cyclesExecuted)
{
    uint16_t previousCounter;

    // Don't update LCD-related registers if the LCD isn't turned on.
    if (lcdControl & 0x80)
        updateLcdStatMode(cyclesExecuted);

    // This is an internal counter that is incremented every clock cycle and determines the DIV and TIMA registers.
    previousCounter = internalCounter;
    internalCounter += cyclesExecuted;

    if (timerControl & 0x04)
    {
        switch (timerControl & 0x03)
        {
            case 0x00:
            {
                if ((previousCounter & 0x0200) && (!(internalCounter & 0x0200)))
                    timerCounter++;
            } break;
            case 0x01:
            {
                if ((previousCounter & 0x0008) && (!(internalCounter & 0x0008)))
                    timerCounter++;
            } break;
            case 0x02:
            {
                if ((previousCounter & 0x0020) && (!(internalCounter & 0x0020)))
                    timerCounter++;
            } break;
            case 0x03:
            {
                if ((previousCounter & 0x0080) && (!(internalCounter & 0x0080)))
                    timerCounter++;
            } break;
        }

        if (timerCounter == 0)
            interruptRequestFlags |= 0x04;
    }
}
