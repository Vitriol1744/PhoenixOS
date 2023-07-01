/*
 * Created by vitriol1744 on 28.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "BootInfo.hpp"

#include <cstddef>
#include <cstdint>

#define HIGHER_HALF_OFFSET BootInfo::GetHHDMOffset()

using PhysAddr = uintptr_t;
using VirtAddr = uintptr_t;

template <typename Address>
inline static constexpr bool IsHigherHalfAddress(Address addr)
{
    return reinterpret_cast<VirtAddr>(addr) >= HIGHER_HALF_OFFSET;
}
template <typename T>
inline static constexpr T  ToHigherHalfAddress(PhysAddr addr)
{
    T ret = IsHigherHalfAddress(addr)
            ? reinterpret_cast<T>(addr)
            : reinterpret_cast<T>(addr + HIGHER_HALF_OFFSET);
    return ret;
}
template <typename T>
inline static constexpr T FromHigherHalfAddress(VirtAddr addr)
{
    T ret = !IsHigherHalfAddress(addr)
            ? reinterpret_cast<T>(addr)
            : reinterpret_cast<T>(addr - HIGHER_HALF_OFFSET);
    return ret;
}