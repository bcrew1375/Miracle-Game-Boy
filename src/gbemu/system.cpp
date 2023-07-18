#include <iostream>
#include <memory>

#include "system.h"


System::System(std::unique_ptr<uint8_t[]> bootROM,
               std::shared_ptr<uint8_t[]> romData,
               uint32_t romSizeInBytes,
               std::shared_ptr<uint8_t[]> saveData,
               uint32_t saveDataSize)
{
    ioPorts = std::make_shared<IOPorts>();
    memoryMap = std::make_shared<MemoryMap>(std::move(bootROM), romData, romSizeInBytes, ioPorts);
    display = std::make_shared<Display>(memoryMap->getVideoRamPointer(), memoryMap->getSpriteAttributeTablePointer(), ioPorts);
    cpu = std::make_shared<CPU>(memoryMap, ioPorts, display);

    clockSpeed = 4194304;
    displayRefreshRate = 59.73; //59.72750056960583;
    cyclesPerFrame = (uint32_t)(clockSpeed / displayRefreshRate);

    if (saveDataSize > 0)
    {
        memoryMap->setSaveRam(saveData, saveDataSize);
    }

    isRunning = true;
}


System::~System()
{
}


bool System::getIsRunning() const
{
    return isRunning;
}


double System::getRefreshRate() const
{
    return displayRefreshRate;
}


std::string System::getSystemError() const
{
    return systemError;
}


std::shared_ptr<uint32_t[]> System::getFrameBuffer() const
{
    return display->getFrameBuffer();
}


uint32_t System::getSaveDataSize() const
{
    return memoryMap->getSaveRamSize();
}


std::shared_ptr<uint8_t[]> System::getSaveData() const
{
    return memoryMap->getSaveRamPointer();
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
