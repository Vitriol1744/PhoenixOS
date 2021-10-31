#ifndef IO_ACCESS_H
#define IO_ACCESS_H

#include "common.h"

inline static byte_t inb(word_t port)
{
    byte_t ret;
    PH_ASM("inb %1, %0" : "=a"(ret) : "Nd"(port));

    return ret;
}
inline static word_t inw(word_t port)
{
    word_t ret;
    PH_ASM("inw %1, %0" : "=a"(ret) : "Nd"(port));

    return ret;
}
inline static dword_t inl(word_t port)
{
    dword_t ret;
    PH_ASM("inl %1, %0" : "=a"(ret) : "Nd"(port));

    return ret;
}

inline static void outb(word_t port, byte_t value)
{
    PH_ASM_VOL("outb %0, %1" : : "a"(value), "Nd"(port));
}
inline static void outw(word_t port, word_t value)
{
    PH_ASM_VOL("outw %0, %1" : : "a"(value), "Nd"(port));
}
inline static void outl(word_t port, dword_t value)
{
    PH_ASM_VOL("outl %0, %1" : : "a"(value), "Nd"(port));
}

inline static void ioWait(void)
{
    outb(0x80, 0);
}

#endif // IO_ACCESS_H