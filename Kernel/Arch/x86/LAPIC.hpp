/*
 * Created by vitriol1744 on 07.12.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Common.hpp"

#include "Arch/x86/IDT.hpp"

inline static constexpr const uint32_t LAPIC_TIMER_ISR = 0x20;

namespace LAPIC
{
    void    Initialize();
    void    SendEOI();
    uint8_t GetCurrentCPU();
    void    OneShot(uint64_t us, InterruptHandler handler);
}; // namespace LAPIC
