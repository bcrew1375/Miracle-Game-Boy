#include "cpu.h"


CPU::CPU(Memory *memory)
{
    this->memory = memory;

    registerAF = 0x01B0;
    registerBC = 0x0013;
    registerDE = 0x00D8;
    registerHL = 0x014D;
    registerSP = 0xFFFE;

    memory->write8bit(0xFF05, 0x00); // TIMA
    memory->write8bit(0xFF06, 0x00); // TMA
    memory->write8bit(0xFF07, 0x00); // TAC
    memory->write8bit(0xFF10, 0x80); // NR10
    memory->write8bit(0xFF11, 0xBF); // NR11
    memory->write8bit(0xFF14, 0xBF); // NR14
    memory->write8bit(0xFF16, 0x3F); // NR21
    memory->write8bit(0xFF17, 0x00); // NR22
    memory->write8bit(0xFF19, 0xBF); // NR24
    memory->write8bit(0xFF1A, 0x7F); // NR30
    memory->write8bit(0xFF1B, 0xFF); // NR31
    memory->write8bit(0xFF1C, 0x9F); // NR32
    memory->write8bit(0xFF1E, 0xBF); // NR34
    memory->write8bit(0xFF20, 0xFF); // NR41
    memory->write8bit(0xFF21, 0x00); // NR42
    memory->write8bit(0xFF22, 0x00); // NR43
    memory->write8bit(0xFF23, 0xBF); // NR44
    memory->write8bit(0xFF24, 0x77); // NR50
    memory->write8bit(0xFF25, 0xF3); // NR51
    memory->write8bit(0xFF26, 0xF1); // [$FF26] = $F1-GB, $F0-SGB ; NR52
    memory->write8bit(0xFF40, 0x91); // LCDC
    memory->write8bit(0xFF42, 0x00); // SCY
    memory->write8bit(0xFF43, 0x00); // SCX
    memory->write8bit(0xFF45, 0x00); // LYC
    memory->write8bit(0xFF47, 0xFC); // BGP
    memory->write8bit(0xFF48, 0xFF); // OBP0
    memory->write8bit(0xFF49, 0xFF); // OBP1
    memory->write8bit(0xFF4A, 0x00); // WY
    memory->write8bit(0xFF4B, 0x00); // WX
    memory->write8bit(0xFFFF, 0x00); // IE
}
