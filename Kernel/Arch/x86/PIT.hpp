/*
 * Created by vitriol1744 on 01.12.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Common.hpp"

inline static constexpr const uint32_t PIT_BASE_FREQUENCY = 1193182;

namespace PIT
{
    void     Initialize();
    void     SetFrequency(size_t frequency);
    void     SetReloadValue(uint16_t reloadValue);
    uint64_t GetCurrentCount();
} // namespace PIT