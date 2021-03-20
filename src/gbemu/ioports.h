#ifndef IOPORTS_H
#define IOPORTS_H

#include <stdint.h>


class IOPorts
{
    public:
        IOPorts();

        uint8_t getLcdControl();
        uint8_t getLcdStatus();
        uint8_t getLcdYCoordinate();
        uint8_t getSerialTransferControl();
        uint8_t getSerialTransferData();
        uint8_t getScrollX();
        uint8_t getScrollY();
        uint8_t getInterruptRequestFlags();
        uint8_t getDivider();
        uint8_t getLcdYCompare();
        uint8_t getTimerControl();
        uint8_t getTimerCounter();
        uint8_t getTimerModulo();
        bool getHBlankBeginFlag();
        void setInterruptRequestFlags(uint8_t data);
        void setLcdControl(uint8_t data);
        void setLcdStatus(uint8_t data);
        void setLcdYCoordinate(uint8_t data);
        void setLcdYCompare(uint8_t data);
        void setSerialTransferControl(uint8_t data);
        void setSerialTransferData(uint8_t data);
        void setScrollX(uint8_t data);
        void setScrollY(uint8_t data);
        void setDivider(uint8_t data);
        void setTimerControl(uint8_t data);
        void setTimerCounter(uint8_t data);
        void setTimerModulo(uint8_t data);
        void updateRegisters(uint16_t cyclesExecuted);
    private:
        void updateLcdStatMode(uint16_t cyclesExecuted);

        uint8_t controller;
        uint8_t serialTransferData;
        uint8_t serialTransferControl;
        uint8_t divider;
        int16_t dividerCycles;
        uint8_t timerCounter;
        uint8_t timerModulo;
        uint8_t timerControl;
        uint8_t soundChannel1Sweep;
        uint8_t soundChannel1Length;
        uint8_t soundChannel1Envelope;
        uint8_t soundChannel1FrequencyLo;
        uint8_t soundChannel2FrequencyHi;
        uint8_t soundChannel2Length;
        uint8_t lcdControl;
        uint8_t lcdStatus;
        uint8_t lcdYCoordinate;
        uint8_t lcdYCompare;
        uint8_t scrollX;
        uint8_t scrollY;
        uint8_t interruptRequestFlags;

        int16_t lcdStatModeCycles;
        bool hBlankBeginFlag;
};

#endif // IOPORTS_H
