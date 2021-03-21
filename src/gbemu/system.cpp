#include <iostream>

#include "system.h"


System::System(uint8_t *bootROM, uint8_t *romData, uint32_t romSizeInBytes)
{
    ioPorts = new IOPorts();
    memory = new Memory(bootROM, romData, romSizeInBytes, ioPorts);
    cpu = new CPU(memory, ioPorts);
    interrupts = new Interrupts(memory, cpu);
    display = new Display(memory->getVideoRamPointer(), memory->getSpriteAttributeTablePointer(), ioPorts);

    clockSpeed = 4194304;
    displayRefreshRate = 59.73;
    cyclesPerFrame = clockSpeed / displayRefreshRate;

    isRunning = true;
}


System::~System()
{
    delete ioPorts;
    delete memory;
    delete cpu;
    delete interrupts;
    delete display;
}


void System::executeCycles() {
    cyclesLeftToRun = cyclesPerFrame;

    while (cyclesLeftToRun > 0) {
        previousOpcode = cpu->getOpcode();
        previousPC = cpu->getRegisterPC();

        cyclesExecuted = cpu->execute();

        // If the CPU ran 0 cycles, it encountered an invalid opcode and has to be terminated.
        if (cyclesExecuted == 0) {
            isRunning = false;
            systemError = "Invalid opcode: " + std::to_string(previousOpcode) + " at PC: " + std::to_string(previousPC);
        }

        ioPorts->updateRegisters(cyclesExecuted);
        if (ioPorts->getHBlankBeginFlag() == true)
            display->createScanline();
        cpu->handleInterrupts();

        cyclesLeftToRun -= cyclesExecuted;
    }
    display->updateDisplayOutput();
}


bool System::getIsRunning()
{
    return isRunning;
}


double System::getRefreshRate() {
    return displayRefreshRate;
}


std::string System::getSystemError()
{
    return systemError;
}


uint32_t *System::getFrameBuffer()
{
    return display->getFrameBuffer();
}
