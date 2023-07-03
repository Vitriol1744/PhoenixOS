/*
 * Created by vitriol1744 on 28.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include <cstddef>

namespace Math
{
    inline constexpr size_t AlignDown(size_t value, size_t alignment)
    {
        return value & ~(alignment - 1);
    }
    inline constexpr size_t AlignUp(size_t value, size_t alignment)
    {
        return AlignDown(value + alignment - 1, alignment);
    }
    constexpr inline auto DivRoundUp(auto value, auto alignment)
    {
        return Math::AlignDown(value + alignment - 1, alignment) / alignment;
    }
    inline constexpr bool IsPowerOfTwo(size_t value)
    {
        return value != 0 && !(value & (value - 1));
    }
} // namespace Math