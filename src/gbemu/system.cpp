#include "System.h"
#include "IoPorts.h"
#include "Cpu.h"
#include "MemoryMap.h"
#include "Display.h"

#include <iostream>
#include <memory>


System::System(std::unique_ptr<const uint8_t[]> bootROM,
               std::unique_ptr<const uint8_t[]> romData,
               const uint32_t romSizeInBytes,
               std::unique_ptr<const uint8_t[]> saveData,
               const uint32_t saveDataSize)
{
    ioPorts = std::make_shared<IOPorts>();
    memoryMap = std::make_shared<MemoryMap>(std::move(bootROM), std::move(romData), romSizeInBytes, ioPorts);
    display = std::make_shared<Display>(memoryMap->getVideoRamArray(), memoryMap->getSpriteAttributeTableArray(), ioPorts);
    cpu = std::make_unique<CPU>(memoryMap, ioPorts, display); 

    if (saveDataSize > 0)
    {
        memoryMap->setSaveRam(std::move(saveData), saveDataSize);
    }

    isRunning = true;
}


System::~System()
{
}


const bool System::getIsRunning() const
{
    return isRunning;
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


std::shared_ptr<const uint8_t[]> System::getSaveData() const
{
    return memoryMap->getSaveRamArray();
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
