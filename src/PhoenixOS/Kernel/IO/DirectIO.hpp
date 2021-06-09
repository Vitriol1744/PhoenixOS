#pragma once

#include "Kernel/Types.hpp"

inline static int InB(uint16 port)
{
    uint8 ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));

    return ret;
}

inline static void OutB(uint16 port, uint8 value)
{
    asm volatile ( "outb %0, %1" : : "a"(value), "Nd"(port) );
}

inline static void IOWait()
{
    asm volatile ("jmp 1f\n\t" "1:jmp 2f\n\t" "2:");
}