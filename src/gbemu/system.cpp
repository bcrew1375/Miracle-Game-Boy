#include <iostream>

#include "system.h"


System::System(uint8_t *bootROM, uint8_t *romData, uint32_t romSizeInBytes, uint8_t *saveData, uint32_t saveDataSize)
{
    ioPorts = new IOPorts();
    memory = new Memory(bootROM, romData, romSizeInBytes, ioPorts);
    display = new Display(memory->getVideoRamPointer(), memory->getSpriteAttributeTablePointer(), ioPorts);
    cpu = new CPU(memory, ioPorts, display);

    clockSpeed = 4194304;
    displayRefreshRate = 59.73; //59.72750056960583;
    cyclesPerFrame = (uint32_t)(clockSpeed / displayRefreshRate);

    memory->setSaveRam(saveData, saveDataSize);

    isRunning = true;
}


System::~System()
{
    delete ioPorts;
    delete memory;
    delete cpu;
    delete display;
}


bool System::getIsRunning() const
{
    return isRunning;
}


double System::getRefreshRate() const
{
    return 500;//displayRefreshRate;
}


std::string System::getSystemError() const
{
    return systemError;
}


uint32_t *System::getFrameBuffer() const
{
    return display->getFrameBuffer();
}


uint32_t System::getSaveDataSize() const
{
    return memory->getSaveRamSize();
}


uint8_t *System::getSaveData() const
{
    return memory->getSaveRamPointer();
}


void System::executeCycles() {
    // TO DO: Transfer the main execution loop to the CPU class to prevent calling "execute" for every instruction.
    cyclesLeftToRun = cpu->execute(cyclesPerFrame);

    // If the CPU ran less cycles than it was supposed to, it encountered an invalid opcode and has to be terminated.
    if (cyclesLeftToRun > 0) {
        previousOpcode = cpu->getOpcode();
        previousPC = cpu->getRegisterPC();
        isRunning = false;
        systemError = "Invalid opcode: " + std::to_string(previousOpcode) + " at PC: " + std::to_string(previousPC);
    }
}


void System::setControllerInputs(bool *buttonInputs)
{
    ioPorts->setControllerInputs(buttonInputs);
}
