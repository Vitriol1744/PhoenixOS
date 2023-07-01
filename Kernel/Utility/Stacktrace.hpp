/*
 * Created by vitriol1744 on 26.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include <cstdint>

namespace Stacktrace
{
    void Initialize();
    void Print(std::size_t maxFrames);
}