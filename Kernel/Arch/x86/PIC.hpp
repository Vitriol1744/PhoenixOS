#pragma once

#include <stdint.h>

namespace PIC
{
    void     Remap(uint8_t masterOffset, uint8_t slaveOffset);

    void     MaskIRQ(uint8_t irq);
    void     UnmaskIRQ(uint8_t irq);
    void     MaskAllIRQs();
    void     UnmaskAllIRQs();

    void     SendEOI(uint8_t irq);

    uint16_t GetIRR();
    uint16_t GetISR();

} // namespace PIC