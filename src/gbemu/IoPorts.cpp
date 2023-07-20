#include "IoPorts.h"

#include <iostream>


IOPorts::IOPorts()
{
    // All values assume bootrom has just finished executing.
    backgroundPalette = 0xFC;
    controller = 0xCF;
    dmaTransfer = 0x00;
    interruptRequestFlags = 0xE0;
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
    soundChannel1Length = 0xBF;
    soundChannel1Sweep = 0x80;
    spritePalette0 = 0x00;
    spritePalette1 = 0x00;

    internalCounter = 0xAC00;
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


uint8_t IOPorts::getBackgroundPalette() const
{
    return backgroundPalette;
}


uint8_t IOPorts::getController() const
{
    return controller;
}


uint8_t IOPorts::getDivider() const
{
    return (internalCounter & 0xFF00) >> 8;
}


uint8_t IOPorts::getDmaTransfer() const
{
    return dmaTransfer;
}


uint8_t IOPorts::getInterruptRequestFlags() const
{
    return interruptRequestFlags;
}


uint8_t IOPorts::getLcdControl() const
{
    return lcdControl;
}


uint8_t IOPorts::getLcdStatus() const
{
    return lcdStatus;
}


uint8_t IOPorts::getLcdYCompare() const
{
    return lcdYCompare;
}


uint8_t IOPorts::getLcdYCoordinate() const
{
    return lcdYCoordinate;
}


uint8_t IOPorts::getScrollX() const
{
    return scrollX;
}


uint8_t IOPorts::getScrollY() const
{
    return scrollY;
}


uint8_t IOPorts::getSerialTransferControl() const
{
    return serialTransferControl;
}


uint8_t IOPorts::getSerialTransferData() const
{
    return serialTransferData;
}


uint8_t IOPorts::getSoundChannel1Length() const
{
    return soundChannel1Length;
}


uint8_t IOPorts::getSoundChannel1Sweep() const
{
    return soundChannel1Sweep;
}


uint8_t IOPorts::getSoundOnOff() const
{
    return soundOnOff;
}


uint8_t IOPorts::getSpritePalette0() const
{
    return spritePalette0;
}


uint8_t IOPorts::getSpritePalette1() const
{
    return spritePalette1;
}


uint8_t IOPorts::getTimerControl() const
{
    return timerControl;
}


uint8_t IOPorts::getTimerCounter() const
{
    return timerCounter;
}


uint8_t IOPorts::getTimerModulo() const
{
    return timerModulo;
}


uint8_t IOPorts::getWindowX() const
{
    return windowX;
}


uint8_t IOPorts::getWindowY() const
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
        lcdStatModeCycles = 80;
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


void IOPorts::updateLcdStatMode(uint16_t cyclesExecuted)
{
    uint8_t lcdStatMode = lcdStatus & 0x03;

    lcdStatModeCycles -= cyclesExecuted;
    if (lcdStatModeCycles <= 0)
    {
        if ((lcdYCoordinate < 144) && (lcdStatMode != 1))
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
            {
                lcdYCoordinate++;
                // Request OAM interrupt if enabled.
                if (lcdStatus & 0x20)
                    interruptRequestFlags |= 0x02;
            }

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
            lcdStatModeCycles += 456;

            // In reality, the coordinate should sit at 153 for approximately 4 clocks but assume it changes immediately.
            if (lcdYCoordinate == 153)
            {
                if (lcdYCoordinate == lcdYCompare)
                    if (lcdStatus & 0x40)
                        interruptRequestFlags |= 0x02;

                lcdYCoordinate = 0;
            }
        }

        else if (lcdYCoordinate == 0)
        {
            // Request OAM interrupt on the transition from V-Blank if enabled.
            lcdStatMode = 0x02;
            if (lcdStatus & 0x20)
                interruptRequestFlags |= 0x02;
            lcdStatModeCycles += 80;
        }

        if ((lcdYCoordinate == lcdYCompare) && ((lcdStatMode == 0x02) || (lcdStatMode == 0x01)))
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
    }


    // Clear the 2 least significant bits and write the new status.
    lcdStatus &= 0xFC;
    lcdStatus |= lcdStatMode;
}


void IOPorts::setControllerInputs(bool *buttonInputs)
{
    memcpy(&this->buttonInputs, buttonInputs, 8);
}


void IOPorts::updateRegisters(uint16_t cyclesExecuted)
{
    uint16_t previousCounter;

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
                    if (((previousCounter + i) & 0x0008) && (!(internalCounter & 0x0008)))
                    {
                        timerCounter++;

                        if (timerCounter == 0)
                            timerCounterOverflow = true;
                    }
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
}
