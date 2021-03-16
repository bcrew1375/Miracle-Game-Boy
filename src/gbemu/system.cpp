#include <iostream>

#include "system.h"


System::System(uint8_t *romData, uint32_t romSizeInBytes)
{
    memory = new Memory(romData, romSizeInBytes);
    cpu = new CPU(memory);
    display = new Display();

    //systemRunning = false;

    clockSpeed = 4194304;
    displayRefreshRate = 59.73;
    cyclesPerFrame = clockSpeed / displayRefreshRate;
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
        cyclesLeftToRun -= cpu->execute();
    }
    cyclesLeftToRun = 0;
}
