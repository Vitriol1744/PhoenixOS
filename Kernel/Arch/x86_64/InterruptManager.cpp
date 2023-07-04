/*
 * Created by vitriol1744 on 03.07.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "Arch/Interrupts/InterruptManager.hpp"

#include "Arch/x86_64/IDT.hpp"

namespace InterruptManager
{
    void Initialize()
    {
        IDT::Initialize();
        IDT::Load();
    }

    void RegisterInterruptHandler(InterruptHandler& interruptHandler)
    {
        IDT::RegisterInterruptHandler(&interruptHandler);
    }
} // namespace InterruptManager