#ifndef CYCLETABLES_H
#define CYCLETABLES_H


static constexpr uint8_t clockCyclesTable [] = {
                                4, 12,  8,  8,  4,  4,  8,  4, 20,  8,  8,  8,  4,  4,  8,  4,  // 0x0x
                                4, 12,  8,  8,  4,  4,  8,  4, 12,  8,  8,  8,  4,  4,  8,  4,  // 0x1x
                                8, 12,  8,  8,  4,  4,  8,  4,  8,  8,  8,  8,  4,  4,  8,  4,  // 0x2x
                                8, 12,  8,  8, 12, 12, 12,  4,  8,  8,  8,  8,  4,  4,  8,  4,  // 0x3x
                                4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // 0x4x
                                4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // 0x5x
                                4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // 0x6x
                                8,  8,  8,  8,  8,  8,  4,  8,  4,  4,  4,  4,  4,  4,  8,  4,  // 0x7x
                                4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // 0x8x
                                4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // 0x9x
                                4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // 0xAx
                                4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,  // 0xBx
                                8, 12, 12, 16, 12, 16,  8, 16,  8, 16, 12,  4, 12, 24,  8, 16,  // 0xCx
                                8, 12, 12,  0, 12, 16,  8, 16,  8, 16, 12,  0, 12,  0,  8, 16,  // 0xDx
                               12, 12,  8,  0,  0, 16,  8, 16, 16,  4, 16,  0,  0,  0,  8, 16,  // 0xEx
                               12, 12,  8,  4,  0, 16,  8, 16, 12,  8, 16,  4,  0,  0,  8, 16 };// 0xFx

static constexpr uint8_t clockCyclesCBTable[] = {
                                8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,  // 0x00
                                8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,  // 0x10
                                8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,  // 0x20
                                8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,  // 0x30
                                8,  8,  8,  8,  8,  8, 12,  8,  8,  8,  8,  8,  8,  8, 12,  8,  // 0x40
                                8,  8,  8,  8,  8,  8, 12,  8,  8,  8,  8,  8,  8,  8, 12,  8,  // 0x50
                                8,  8,  8,  8,  8,  8, 12,  8,  8,  8,  8,  8,  8,  8, 12,  8,  // 0x60
                                8,  8,  8,  8,  8,  8, 12,  8,  8,  8,  8,  8,  8,  8, 12,  8,  // 0x70
                                8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,  // 0x80
                                8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,  // 0x90
                                8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,  // 0xA0
                                8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,  // 0xB0
                                8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,  // 0xC0
                                8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,  // 0xD0
                                8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,  // 0xE0
                                8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8 };// 0xF0
#endif // CYCLETABLES_H
