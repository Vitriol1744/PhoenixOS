#pragma once

#include "Arch/x86/Types.hpp"

namespace IO
{
    template <typename T>
    inline static void Out(word port, T value) requires(sizeof(T) <= 4)
    {
        if constexpr (sizeof(T) == 1)
            __asm__ volatile("out dx, al" : : "a"(value), "d"(port));
        else if constexpr (sizeof(T) == 2)
            __asm__ volatile("out dx, ax" : : "a"(value), "d"(port));
        else if constexpr (sizeof(T) == 4)
            __asm__ volatile("out dx, eax" : : "a"(value), "d"(port));
    }
    template <typename T>
    inline static T In(word port) requires(sizeof(T) <= 4)
    {
        T value = 0;
        if constexpr (sizeof(T) == 1)
            __asm__ volatile("in al, dx" : "=a"(value) : "d"(port));
        else if constexpr (sizeof(T) == 2)
            __asm__ volatile("in ax, dx" : "=a"(value) : "d"(port));
        else if constexpr (sizeof(T) == 4)
            __asm__ volatile("in eax, dx" : "=a"(value) : "d"(port));

        return value;
    }
    inline static void Wait() { Out<byte>(0x80, 0x00); }
}