/*
 * Created by vitriol1744 on 29.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Arch/Interrupts/InterruptHandler.hpp"

#include <cstddef>

inline constexpr const uint32_t DPL_RING0 = 0x00;
inline constexpr const uint32_t DPL_RING3 = 0x03;

namespace IDT
{
    void Initialize();
    void Load();

    void RegisterInterruptHandler(InterruptHandler* handler, uint8_t dpl = DPL_RING0);
}