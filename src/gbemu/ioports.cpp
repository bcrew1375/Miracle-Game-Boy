#include <iostream>

#include "ioports.h"


IOPorts::IOPorts(uint32_t cyclesPerFrame)
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
    lcdStatus = 0x85;
    lcdYCoordinate = 0x00;
    lcdYCompare = 0x00;
    scrollY = 0;
    scrollX = 0;
    serialTransferControl = 0x7E;
    serialTransferData = 0x00;
    soundChannel1Length = 0xBF;
    soundChannel1Sweep = 0x80;
    spritePalette0 = 0x00;
    spritePalette1 = 0x00;

    this->cyclesPerFrame = cyclesPerFrame;
    previousCycleState = cyclesPerFrame;
    internalCounter = 0xABCC;
    hBlankBeginFlag = false;
    //lcdYCoordinateChangeFlag = false;
    lcdStatModeCycles = 248;
    timerCounterOverflow = false;
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


uint8_t IOPorts::getSoundChannel1Length()
{
    return soundChannel1Length;
}


uint8_t IOPorts::getSoundChannel1Sweep()
{
    return soundChannel1Sweep;
}


uint8_t IOPorts::getSoundOnOff()
{
    return soundOnOff;
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


uint8_t IOPorts::getWindowX()
{
    return windowX;
}


uint8_t IOPorts::getWindowY()
{
    return windowY;
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

        controller |= (uint8_t)(buttonInputs[0] << 3); // Down
        controller |= (uint8_t)(buttonInputs[1] << 2); // Up
        controller |= (uint8_t)(buttonInputs[2] << 1); // Left
        controller |= (uint8_t)(buttonInputs[3]);      // Right

        controller ^= 0x0F;
    }
    if (!(controller & 0x20))
    {
        controller &= 0xF0;

        controller |= (uint8_t)(buttonInputs[4] << 3); // Start
        controller |= (uint8_t)(buttonInputs[5] << 2); // Select
        controller |= (uint8_t)(buttonInputs[6] << 1); // B
        controller |= (uint8_t)(buttonInputs[7]);      // A

        controller ^= 0x0F;
    }
}


void IOPorts::setDivider()
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
        lcdStatModeCycles = 0;
    }
}

void IOPorts::setLcdStatus(uint8_t data)
{
    // Make sure the current mode isn't overwritten.
    lcdStatus &= 0x83;
    lcdStatus |= (data & 0xFC);
}


void IOPorts::setLcdYCompare(uint8_t data)
{
    lcdYCompare = data;

    if (lcdYCoordinate == lcdYCompare) {
        lcdStatus |= 0x04;
        if (lcdStatus & 0x40)
            interruptRequestFlags |= 0x02;
    }
}


void IOPorts::setLcdYCoordinate()
{
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
    // Assume there is no other connected Game Boy.
    serialTransferControl = data & 0x7F;

    if (serialTransferControl & 0x01)
    {
        serialTransferData = 0xFF;
        interruptRequestFlags |= 0x08;
    }
}


void IOPorts::setSerialTransferData(uint8_t data)
{
    serialTransferData = data;
}


void IOPorts::setSoundChannel1Length(uint8_t data)
{
    soundChannel1Length = data;
}


void IOPorts::setSoundChannel1Sweep(uint8_t data)
{
    soundChannel1Sweep = data;
}


void IOPorts::setSoundOnOff(uint8_t data)
{
    soundOnOff = data & 0x80;
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

    /*if (timerControl & 0x04) {
        switch (timerControl & 0x03) {
        case 0x00: timerCyclesReset = 1024; break;
        case 0x01: timerCyclesReset = 16; break;
        case 0x02: timerCyclesReset = 64; break;
        case 0x03: timerCyclesReset = 256; break;
        }

        timerCycles = timerCyclesReset;
    }*/
}


void IOPorts::setTimerCounter(uint8_t data)
{
    timerCounter = data;
}


void IOPorts::setTimerModulo(uint8_t data)
{
    timerModulo = data;
}


void IOPorts::setWindowX(uint8_t data)
{
    windowX = data;
}


void IOPorts::setWindowY(uint8_t data)
{
    windowY = data;
}


void IOPorts::updateLcdStatMode(uint32_t cyclesExecuted)
{
    bool lcdStatModeChange = false;
    bool lcdYCoordinateChangeFlag = false;

    uint16_t previousLcdStatModeCycles;
    uint8_t lcdStatMode = lcdStatus & 0x03;

    previousLcdStatModeCycles = lcdStatModeCycles;

    lcdStatModeCycles += cyclesExecuted;
    lcdStatModeCycles %= 456;


    // Indicates the cycles have wrapped. Mode 2 begins and LY increments.
    if (previousLcdStatModeCycles > lcdStatModeCycles)
    {
        // LY coordinate 0 should be present for stat mode 1 and 2.
        if ((lcdYCoordinate > 0) || ((lcdYCoordinate == 0) && (lcdStatMode != 1)))
        {
            lcdYCoordinate++;
            lcdYCoordinateChangeFlag = true;
        }
        else
            lcdStatMode = 2;
    }

    if ((lcdYCoordinate < 144) && (lcdStatMode != 1))
    {
        if (lcdStatModeCycles < 80)
        {
            if (lcdStatMode != 2)
            {
                lcdStatMode = 2;
                lcdStatModeChange = true;
            }
        }
        else if ((lcdStatModeCycles >= 80) && (lcdStatModeCycles < 248))
        {
            if (lcdStatMode != 3)
            {
                lcdStatMode = 3;
                lcdStatModeChange = true;
            }
        }
        else if (lcdStatModeCycles >= 248)
        {
            if (lcdStatMode != 0)
            {
                lcdStatMode = 0;
                lcdStatModeChange = true;
            }
        }
    }

    if ((lcdStatModeChange == true) && (lcdYCoordinate < 144))
    {
        if (lcdStatMode == 0)
        {
            // Request H-Blank interrupt if enabled.
            if (lcdStatus & 0x08)
                interruptRequestFlags |= 0x02;
            hBlankBeginFlag = true;
        }

        if (lcdStatMode == 2)
        {
            // Request OAM interrupt if enabled.
            if (lcdStatus & 0x20)
                interruptRequestFlags |= 0x02;
        }
    }

    if ((lcdYCoordinateChangeFlag == true) && (lcdYCoordinate == 144))
    {
        // V-Blank begins.
        lcdStatMode = 1;
        interruptRequestFlags |= 0x01;
        if (lcdStatus & 0x10)
            interruptRequestFlags |= 0x02;
    }

    else if (lcdYCoordinate == 153)
    {
        if (lcdYCoordinate == lcdYCompare)
            if (lcdStatus & 0x40)
                interruptRequestFlags |= 0x02;

        // In reality, the coordinate should sit at 153 for approximately 4 clocks but assume it changes immediately.
        lcdYCoordinate = 0;
    }

    if ((lcdYCoordinateChangeFlag == true) && (lcdYCoordinate == lcdYCompare))
    {
        if (lcdYCoordinate != 0)
        {
            lcdStatus |= 0x04;
            if (lcdStatus & 0x40)
                interruptRequestFlags |= 0x02;
        }
        else
        {
            // Since the LY coordinate can be 0 in both modes 1 and 2, make sure the compare is only made during mode 1.
            if (lcdStatMode == 1)
            {
                lcdStatus |= 0x04;
                if (lcdStatus & 0x40)
                    interruptRequestFlags |= 0x02;
            }
        }
    }
    else
        lcdStatus &= 0xFB;

    // Clear the 2 least significant bits and write the new status.
    lcdStatus &= 0xFC;
    lcdStatus |= lcdStatMode;
}


void IOPorts::setControllerInputs(bool *buttonInputs)
{
    memcpy(&this->buttonInputs, buttonInputs, 8);
}


void IOPorts::updateRegisters(int32_t cyclesExecuted)
{
    uint16_t previousCounter;

    currentCycleState = previousCycleState - cyclesExecuted;
    if (currentCycleState <= 0)
        currentCycleState += cyclesPerFrame;

    // Don't update LCD-related registers if the LCD isn't turned on.
    if (lcdControl & 0x80)
        updateLcdStatMode(cyclesExecuted);

    // This is an internal counter that is incremented for every clock cycle and determines the DIV and TIMA registers.
    previousCounter = internalCounter;
    internalCounter += cyclesExecuted;

    if (timerControl & 0x04)
    {
        // Delay the timer reset and interrupt request by at least 4 cycles.
        if (timerCounterOverflow == true)
        {
            timerCounterOverflow = false;
            timerCounter = timerModulo;
            interruptRequestFlags |= 0x04;
        }

        switch (timerControl & 0x03)
        {
            case 0x00:
            {
                // Check for an overflow from bit 9.
                if ((previousCounter & 0x0200) && (!(internalCounter & 0x0200)))
                {
                    timerCounter++;

                    if (timerCounter == 0)
                        timerCounterOverflow = true;
                }
            } break;
            case 0x01:
            {
                // An exception for cases where the timer counter can increase twice on one instruction
                // due to the timer's speed at this setting.
                for (int i = 0; i < cyclesExecuted; i += 4)
                {
                    // Check for an overflow from bit 3.
                    if ((previousCounter & 0x0008) && (!((previousCounter + 4) & 0x0008)))
                    {
                        timerCounter++;

                        if (timerCounter == 0)
                            timerCounterOverflow = true;
                    }
                    previousCounter += 4;
                }
            } break;
            case 0x02:
            {
                // Check for an overflow from bit 5.
                if ((previousCounter & 0x0020) && (!(internalCounter & 0x0020)))
                {
                    timerCounter++;

                    if (timerCounter == 0)
                        timerCounterOverflow = true;
                }
            } break;
            case 0x03:
            {
                // Check for an overflow from bit 7.
                if ((previousCounter & 0x0080) && (!(internalCounter & 0x0080)))
                {
                    timerCounter++;

                    if (timerCounter == 0)
                        timerCounterOverflow = true;
                }
            } break;
        }
    }

    previousCycleState = currentCycleState;
}
