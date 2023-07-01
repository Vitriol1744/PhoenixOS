/*
 * Created by vitriol1744 on 26.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include <cstddef>
#include <cstdint>

namespace Serial
{
    bool        Initialize();

    uint8_t     Read();
    void        Write(uint8_t data);
    inline void LogString(const char* str, size_t len)
    {
        while (len-- > 0) Write(*str++);
    }
} // namespace Serial