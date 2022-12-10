/*
 * Created by vitriol1744 on 26.11.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "PIC.hpp"

#include "Common.hpp"

#include "Arch/x86/IO.hpp"

// https://pdos.csail.mit.edu/6.828/2005/readings/hardware/8259A.pdf
inline static constexpr const uint32_t PIC1          = 0x20;
inline static constexpr const uint32_t PIC1_COMMAND  = PIC1 + 0;
inline static constexpr const uint32_t PIC1_DATA     = PIC1 + 1;

inline static constexpr const uint32_t PIC2          = 0xa0;
inline static constexpr const uint32_t PIC2_COMMAND  = PIC2 + 0;
inline static constexpr const uint32_t PIC2_DATA     = PIC2 + 1;

inline static constexpr const uint32_t OCW2_SEND_EOI = 0x20;

inline static constexpr const uint32_t OCW3_GET_IRR  = 0x0a;
inline static constexpr const uint32_t OCW3_GET_ISR  = 0x0b;

enum class ICW1Flags : uint32_t
{
    eICW4Needed         = BIT(0),
    // Single mode if set, cascade mode otherwise
    eSingleMode         = BIT(1),
    // If set address interval is 4, otherwise it's 8
    eAddressInterval4   = BIT(2),
    // Level triggered mode if set, edge triggered mode otherwise
    eLevelTriggeredMode = BIT(3),
    // 1 means that it is Initialization Command Word
    eICW                = BIT(4),
};
enum class ICW4Flags : uint32_t
{
    e8086Mode               = BIT(0),
    eAutoEOI                = BIT(1),
    eBufferedModeSlave      = BIT(3),
    eBufferedModeMaster     = BIT(2) | BIT(3),
    eSpecialFullyNestedMode = BIT(4),
};

ICW1Flags operator|(ICW1Flags lhs, ICW1Flags rhs)
{
    uint32_t ret = static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs);

    return static_cast<ICW1Flags>(ret);
}

namespace PIC
{
    static uint16_t GetIRQRegister(uint8_t ocw3)
    {
        IO::Out<byte>(PIC1_COMMAND, ocw3);
        IO::Out<byte>(PIC2_COMMAND, ocw3);

        return (IO::In<byte>(PIC2_COMMAND) << 8) | IO::In<byte>(PIC1_COMMAND);
    }

    void Remap(uint8_t masterOffset, uint8_t slaveOffset)
    {
        // Save Masks
        uint8_t  pic1Mask = IO::In<byte>(PIC1_DATA);
        uint8_t  pic2Mask = IO::In<byte>(PIC2_DATA);

        uint32_t icw1
            = static_cast<uint32_t>(ICW1Flags::eICW | ICW1Flags::eICW4Needed);
        // ICW1: Begin initialization sequence
        IO::Out<byte>(PIC1_COMMAND, icw1);
        IO::Wait();
        IO::Out<byte>(PIC2_COMMAND, icw1);
        IO::Wait();

        // ICW2: Set PIC offsets
        IO::Out<byte>(PIC1_DATA, masterOffset);
        IO::Wait();
        IO::Out<byte>(PIC2_DATA, slaveOffset);
        IO::Wait();

        // ICW3: Initialize Cascade Mode
        IO::Out<byte>(PIC1_DATA, 0x04);
        IO::Wait();
        IO::Out<byte>(PIC2_DATA, 0x02);
        IO::Wait();

        // ICW4: Set x86 mode
        uint32_t icw4 = static_cast<uint32_t>(ICW4Flags::e8086Mode);
        IO::Out<byte>(PIC1_DATA, icw4);
        IO::Wait();
        IO::Out<byte>(PIC2_DATA, icw4);
        IO::Wait();

        // Restore Masks
        IO::Out<byte>(PIC1_DATA, pic1Mask);
        IO::Out<byte>(PIC2_DATA, pic2Mask);
    }

    void MaskIRQ(uint8_t irq)
    {
        uint8_t picPort = PIC1_DATA;
        if (irq >= 8)
        {
            irq -= 8;
            picPort = PIC2_DATA;
        }

        uint8_t ocw1 = IO::In<byte>(picPort) | (1 << irq);
        IO::Out<byte>(picPort, ocw1);
    }
    void UnmaskIRQ(uint8_t irq)
    {
        uint8_t picPort = PIC1_DATA;
        if (irq >= 8)
        {
            irq -= 8;
            picPort = PIC2_DATA;
        }

        uint8_t ocw1 = IO::In<byte>(picPort) & ~(1 << irq);
        IO::Out<byte>(picPort, ocw1);
    }
    void MaskAllIRQs()
    {
        IO::Out<byte>(PIC1_DATA, 0xff);
        IO::Out<byte>(PIC2_DATA, 0xff);
    }
    void UnmaskAllIRQs()
    {
        IO::Out<byte>(PIC1_DATA, 0x00);
        IO::Out<byte>(PIC2_DATA, 0x00);
    }

    // Returns true if interrupt was spurious
    static bool HandleSpuriousInterrupt(uint8_t irq)
    {
        if (irq != 8 && irq != 15) return false;
        if (GetISR() & (1 << irq)) return false;

        if (irq == 15) IO::Out<byte>(PIC1_COMMAND, OCW2_SEND_EOI);

        return true;
    }
    void SendEOI(uint8_t irq)
    {
        if (HandleSpuriousInterrupt(irq)) return;

        if (irq >= 8) IO::Out<byte>(PIC2_COMMAND, OCW2_SEND_EOI);
        IO::Out<byte>(PIC1_COMMAND, OCW2_SEND_EOI);
    }

    uint16_t GetIRR() { return GetIRQRegister(OCW3_GET_IRR); }
    uint16_t GetISR() { return GetIRQRegister(OCW3_GET_ISR); }

} // namespace PIC