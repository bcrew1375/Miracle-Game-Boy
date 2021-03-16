#include <iostream>

#include "system.h"


System::System(uint8_t *romData, uint32_t romSizeInBytes)
{
    memory = new Memory(romData, romSizeInBytes);
    cpu = new CPU(memory);
    interrupts = new Interrupts(memory, cpu);
    display = new Display();

    clockSpeed = 4194304;
    displayRefreshRate = 59.73;
    cyclesPerFrame = clockSpeed / displayRefreshRate;

    isRunning = true;
}


/*void System::runSystem(bool systemRunning) {
    this->systemRunning = systemRunning;
}*/


/*bool System::isRunning() {
    return systemRunning;
}*/


double System::getRefreshRate() {
    return displayRefreshRate;
}


void System::executeCycles() {
    cyclesLeftToRun = cyclesPerFrame;

    while (cyclesLeftToRun > 0) {
        cyclesBeforeExecution = cyclesLeftToRun;
        previousOpcode = cpu->getOpcode();
        previousPC = cpu->getRegisterPC();
        cyclesLeftToRun -= cpu->execute();
        // If the CPU ran 0 cycles, it encountered an invalid opcode and has to be terminated.
        if (cyclesBeforeExecution == cyclesLeftToRun) {
            isRunning = false;
            systemError = "Invalid opcode: " + std::to_string(previousOpcode) + " at PC: " + std::to_string(previousPC);
        }
    }
    cyclesLeftToRun = 0;
}


bool System::getIsRunning()
{
    return isRunning;
}


std::string System::getSystemError()
{
    return systemError;
}
