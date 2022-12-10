/*
 * Created by vitriol1744 on 26.11.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Arch/x86/Types.hpp"

namespace IO
{
    template <typename T>
    inline static void Out(word port, T value) requires(sizeof(T) <= 4)
    {
        if constexpr (sizeof(T) == 1)
            __asm__ volatile("outb %0, %1" : : "a"(value), "d"(port));
        else if constexpr (sizeof(T) == 2)
            __asm__ volatile("outw %0, %1" : : "a"(value), "d"(port));
        else if constexpr (sizeof(T) == 4)
            __asm__ volatile("outl %0, %1" : : "a"(value), "d"(port));
    }
    template <typename T>
    inline static T In(word port) requires(sizeof(T) <= 4)
    {
        T value = 0;
        if constexpr (sizeof(T) == 1)
            __asm__ volatile("inb %1, %0" : "=a"(value) : "d"(port));
        else if constexpr (sizeof(T) == 2)
            __asm__ volatile("inw %1, %0" : "=a"(value) : "d"(port));
        else if constexpr (sizeof(T) == 4)
            __asm__ volatile("inl %1, %0" : "=a"(value) : "d"(port));

        return value;
    }
    inline static void Wait() { Out<byte>(0x80, 0x00); }
}