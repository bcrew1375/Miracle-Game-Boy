#include "interrupts.h"

Interrupts::Interrupts(Memory *memory, CPU *cpu)
{
    this->memory = memory;
    this->cpu = cpu;
}
