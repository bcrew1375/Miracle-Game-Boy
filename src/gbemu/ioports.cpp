#include "ioports.h"


IOPorts::IOPorts()
{
    // All values assume bootrom has just finished executing.
    controller = 0xFF;
    divider = 0xAB;
    dividerCycles = 176;
    interruptRequestFlags = 0xE1;
    timerControl = 0xF8;
    timerCounter = 0x00;
    timerCycles = 0;
    timerCyclesReset = 0;
    timerModulo = 0x00;
    lcdControl = 0x91;
    lcdStatus = 0x82;
    lcdStatModeCycles = 80;
    lcdYCoordinate = 0x00;
    serialTransferControl = 0x7E;
    serialTransferData = 0x00;
    scrollY = 0;
    scrollX = 0;
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


uint8_t IOPorts::getController()
{
    return controller;
}


uint8_t IOPorts::getDivider()
{
    return divider;
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


void IOPorts::setController(uint8_t data)
{
    // Only bits 4-5 are writeable. Bits 0-3 are set hi/lo depending on controller inputs. Bits 6-7 are unusable.
    controller &= 0xCF;
    controller |= data & 0b00110000;
}


void IOPorts::setDivider(uint8_t data)
{
    divider = 0;
    timerCounter = 0;
}


void IOPorts::setInterruptRequestFlags(uint8_t data)
{
    interruptRequestFlags = 0xE0 | data;
}


void IOPorts::setLcdControl(uint8_t data)
{
    lcdControl = data;
}

void IOPorts::setLcdStatus(uint8_t data)
{
    // Make sure the current mode isn't overwritten.
    lcdStatus |= (data & 0xFC);
}


void IOPorts::setLcdYCompare(uint8_t data)
{
    lcdYCoordinate = data;
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


void IOPorts::setTimerControl(uint8_t data)
{
    timerControl = 0xF8 | data;

    if (timerControl & 0x04) {
        switch (timerControl & 0x03) {
        case 0x00: timerCyclesReset = 1024; break;
        case 0x01: timerCyclesReset = 16; break;
        case 0x02: timerCyclesReset = 64; break;
        case 0x03: timerCyclesReset = 256; break;
        }

        timerCycles = timerCyclesReset;
    }
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
    if ((lcdStatModeCycles <= 0) && (lcdYCoordinate < 144)) {
        switch (lcdStatMode) {
        case 0: lcdStatMode = 2; lcdStatModeCycles += 80; break;
        case 2: lcdStatMode = 3; lcdStatModeCycles += 168; break; // Variable between 168 and 291 depending on sprite count. Assume 168 for now.
        case 3: lcdStatMode = 0; lcdStatModeCycles += 208; break; // Variable between 85 and 208 depending on time taken for mode 3.
        }

        if (lcdStatMode == 0x02)
            lcdYCoordinate++;
        if (lcdStatMode == 0x00) {
            // Request H-Blank interrupt if enabled.
            if (lcdStatus & 0x08)
                interruptRequestFlags |= 0x02;
            hBlankBeginFlag = true;
        }
    }
    else if ((lcdYCoordinate == 144) && (lcdStatMode == 0x02)) {
        lcdStatMode = 0x01;
        lcdStatModeCycles += 376; // Account for the 80 cycles from mode 2 by not adding them here.
        interruptRequestFlags |= 0x01;
        if (lcdStatus & 0x10)
            interruptRequestFlags |= 0x02;
    }
    else if ((lcdStatModeCycles <= 0) && (lcdYCoordinate >= 144)) {
        lcdYCoordinate++;
        if (lcdYCoordinate == 154) {
            lcdYCoordinate = 0;
            lcdStatMode = 0x02;
            lcdStatModeCycles += 80;
        }
        else {
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
    // Don't update LCD-related registers if the LCD isn't turned on.
    if (lcdControl & 0x80)
        updateLcdStatMode(cyclesExecuted);

    dividerCycles -= cyclesExecuted;
    if (dividerCycles < 0) {
        divider++;
        dividerCycles += 256;
    }

    if (timerControl & 0x04) {
        timerCycles -= cyclesExecuted;
        if (timerCycles < 0) {
            timerCycles = timerCyclesReset;

            timerCounter++;

            if (timerCounter == 0) {
                timerCounter = timerModulo;
                interruptRequestFlags |= 0x04;
            }
        }
    }
}
