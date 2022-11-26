#pragma once

#include <stdint.h>

namespace PIC
{
    void Remap(uint8_t masterOffset, uint8_t slaveOffset);
    void SendEOI(uint8_t irq);
    void MaskAll();
}