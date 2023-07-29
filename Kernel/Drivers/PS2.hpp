/*
 * Created by vitriol1744 on 18.07.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Common.hpp"

namespace PS2
{
    void Initialize();

    void Write(u16 port, u8 value);
    u8   Read();

    void WriteConfig(u8 value);
    u8   ReadConfig();
} // namespace PS2