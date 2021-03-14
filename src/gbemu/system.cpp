#include "system.h"

#include <iostream>
System::System(uint8_t *romData, uint32_t romSizeInBytes)
{
    memory = new Memory(romData, romSizeInBytes);
    cpu = new CPU(memory);
    display = new Display();
}
