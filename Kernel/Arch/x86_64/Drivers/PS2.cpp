/*
 * Created by vitriol1744 on 18.07.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "Drivers/PS2.hpp"

#include "Arch/x86_64/CPU.hpp"
#include "Arch/x86_64/Drivers/IOAPIC.hpp"
#include "Arch/x86_64/IDT.hpp"
#include "Arch/x86_64/IO.hpp"

namespace PS2
{
    static u8 keyboardVector = 0xff;

    void      Initialize()
    {
        Write(0x64, 0xad);
        Write(0x64, 0xa7);

        u8 ps2Config = ReadConfig();
        ps2Config |= BIT(0) | BIT(6);
        if ((ps2Config & BIT(5)) != 0) ps2Config |= BIT(1);
        WriteConfig(ps2Config);

        Write(0x64, 0xae);
        if ((ps2Config & BIT(5)) != 0) Write(0x64, 0xa8);

        keyboardVector = IDT::AllocateVector();
        IOAPIC::SetIRQRedirect(CPU::GetBSP().lapicID, keyboardVector, 1, true);
        IO::In<byte>(0x60);
    }

    void Write(u16 port, u8 value)
    {
        while ((IO::In<byte>(0x64) & 2) != 0)
            ;
        IO::Out<byte>(port, value);
    }
    u8 Read()
    {
        while ((IO::In<byte>(0x64) & 1) == 0)
            ;
        return IO::In<byte>(0x60);
    }

    void WriteConfig(u8 value)
    {
        Write(0x64, 0x60);
        Write(0x60, value);
    }
    u8 ReadConfig()
    {
        Write(0x64, 0x20);
        return Read();
    }
} // namespace PS2