/*
 * Created by vitriol1744 on 25.11.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Arch/x86/CPU.hpp"

#include <stddef.h>

using InterruptHandler                    = void (*)(CPUContext* context);

inline constexpr const uint32_t DPL_RING0 = 0x00;
inline constexpr const uint32_t DPL_RING1 = 0x01;
inline constexpr const uint32_t DPL_RING2 = 0x02;
inline constexpr const uint32_t DPL_RING3 = 0x03;

namespace IDT
{
    void Initialize();
    void Load();

    void RegisterInterruptHandler(uint32_t vector, InterruptHandler handler,
                                  uint8_t dpl = DPL_RING0);
} // namespace IDT