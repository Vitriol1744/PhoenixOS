#include "PIC.hpp"

#include "Arch/x86/IO.hpp"

inline constexpr const uint32_t PIC1         = 0x20;
inline constexpr const uint32_t PIC1_COMMAND = PIC1 + 0;
inline constexpr const uint32_t PIC1_DATA    = PIC1 + 1;

inline constexpr const uint32_t PIC2         = 0xa0;
inline constexpr const uint32_t PIC2_COMMAND = PIC2 + 0;
inline constexpr const uint32_t PIC2_DATA    = PIC2 + 1;

inline constexpr const uint32_t PIC_EOI      = 0x20;

namespace PIC
{
    void Remap(uint8_t masterOffset, uint8_t slaveOffset)
    {
        uint8_t pic1Mask = IO::In<byte>(PIC1_DATA);
        uint8_t pic2Mask = IO::In<byte>(PIC2_DATA);
    }
    void SendEOI(uint8_t irq)
    {
        if (irq >= 8) IO::Out<byte>(PIC2_COMMAND, PIC_EOI);
        IO::Out<byte>(PIC1_COMMAND, PIC_EOI);
    }
    void MaskAll()
    {
        IO::Out<byte>(PIC1, 0xff);
        IO::Out<byte>(PIC2, 0xff);
    }
} // namespace PIC