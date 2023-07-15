#ifndef IOPORTS_H
#define IOPORTS_H

#include <stdint.h>


class IOPorts
{
    public:
        IOPorts();

        bool getHBlankBeginFlag();

        uint8_t getBackgroundPalette() const;
        uint8_t getController() const;
        uint8_t getDivider() const;
        uint8_t getDmaTransfer() const;
        uint8_t getInterruptRequestFlags() const;
        uint8_t getLcdControl() const;
        uint8_t getLcdStatus() const;
        uint8_t getLcdYCompare() const;
        uint8_t getLcdYCoordinate() const;
        uint8_t getScrollX() const;
        uint8_t getScrollY() const;
        uint8_t getSerialTransferControl() const;
        uint8_t getSerialTransferData() const;
        uint8_t getSoundChannel1Length() const;
        uint8_t getSoundChannel1Sweep() const;
        uint8_t getSoundOnOff() const;
        uint8_t getSpritePalette0() const;
        uint8_t getSpritePalette1() const;
        uint8_t getTimerControl() const;
        uint8_t getTimerCounter() const;
        uint8_t getTimerModulo() const;
        uint8_t getWindowX() const;
        uint8_t getWindowY() const;

        void setBackgroundPalette(uint8_t data);
        void setController(uint8_t data);
        void setDivider();
        void setDmaTransfer(uint8_t data, uint8_t *spriteAttributeTable);
        void setInterruptRequestFlags(uint8_t data);
        void setLcdControl(uint8_t data);
        void setLcdStatus(uint8_t data);
        void setLcdYCompare(uint8_t data);
        void setLcdYCoordinate();
        void setScrollX(uint8_t data);
        void setScrollY(uint8_t data);
        void setSerialTransferControl(uint8_t data);
        void setSerialTransferData(uint8_t data);
        void setSoundChannel1Length(uint8_t data);
        void setSoundChannel1Sweep(uint8_t data);
        void setSoundOnOff(uint8_t data);
        void setSpritePalette0(uint8_t data);
        void setSpritePalette1(uint8_t data);
        void setTimerControl(uint8_t data);
        void setTimerCounter(uint8_t data);
        void setTimerModulo(uint8_t data);
        void setWindowX(uint8_t data);
        void setWindowY(uint8_t data);

        void setControllerInputs(bool *buttonInputs);
        void updateRegisters(uint16_t cyclesExecuted);

    private:
        void updateLcdStatMode(uint16_t cyclesExecuted);

        uint8_t backgroundPalette;
        uint8_t controller;
        uint8_t dmaTransfer;
        uint8_t interruptRequestFlags;
        uint8_t lcdControl;
        uint8_t lcdStatus;
        uint8_t lcdYCompare;
        uint8_t lcdYCoordinate;
        uint8_t scrollX;
        uint8_t scrollY;
        uint8_t serialTransferControl;
        uint8_t serialTransferData;
        uint8_t soundChannel1Envelope;
        uint8_t soundChannel1FrequencyLo;
        uint8_t soundChannel1Length;
        uint8_t soundChannel1Sweep;
        uint8_t soundChannel2FrequencyHi;
        uint8_t soundChannel2Length;
        uint8_t soundOnOff;
        uint8_t spriteAttributeTable[0xA0];
        uint8_t spritePalette0;
        uint8_t spritePalette1;
        uint8_t timerControl;
        uint8_t timerCounter;
        uint8_t timerModulo;
        uint8_t windowX;
        uint8_t windowY;

        bool buttonInputs[8] = { false, false, false, false, false, false, false, false };
        bool hBlankBeginFlag;
        bool timerCounterOverflow;

        int32_t lcdStatModeCycles;

        uint16_t internalCounter;
};

#endif // IOPORTS_H
