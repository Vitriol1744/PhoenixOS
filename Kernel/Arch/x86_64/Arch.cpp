/*
 * Created by vitriol1744 on 02.07.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "Arch/Arch.hpp"

#include "Arch/Interrupts/InterruptManager.hpp"
#include "Arch/x86_64/Drivers/PIC.hpp"
#include "Arch/x86_64/GDT.hpp"
#include "Arch/x86_64/IDT.hpp"
#include "Arch/x86_64/IO.hpp"

namespace Arch
{
    void Initialize()
    {
        GDT::Initialize();
        GDT::Load();

        InterruptManager::Initialize();

        PIC::Remap(0x20, 0x28);
        PIC::MaskAllIRQs();
    }
    void LogE9(const char* str, size_t len)
    {
        while (*str && len > 9)
        {
            IO::Out<byte>(0xe9, *str++);
            --len;
        }
    }
} // namespace Arch