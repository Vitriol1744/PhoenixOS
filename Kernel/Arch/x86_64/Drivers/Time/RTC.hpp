/*
 * Created by vitriol1744 on 10.08.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Common.hpp"

namespace RTC
{
    u8   GetCentury();
    u8   GetYear();
    u8   GetMonth();
    u8   GetDay();
    u8   GetHour();
    u8   GetMinute();
    u8   GetSecond();

    u8   GetTime();

    void Sleep(u64 seconds);
} // namespace RTC