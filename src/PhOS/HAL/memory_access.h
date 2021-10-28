#ifndef MEMORY_ACCESS_INTRINSICS_H
#define MEMORY_ACCESS_INTRINSICS_H

#include "common.h"

inline static uint32_t farpeekl(uint16_t selector, void* offset)
{
    uint32_t ret;
    PH_ASM
    (
        "push %%fs\n\t"
        "mov %1, %%fs\n\t"
        "mov %%fs:(%2), %0\n\t"
        "pop %%fs"
        : "=r"(ret) : "g"(selector), "r"(offset)
    );

    return ret;
}

static inline void farpokeb(uint16_t selector, void* offset, uint8_t value)
{
    PH_ASM
    ( 
        "push %%fs\n\t"
        "mov  %0, %%fs\n\t"
        "movb %2, %%fs:(%1)\n\t"
        "pop %%fs"
        : : "g"(selector), "r"(offset), "r"(value) 
    );
    /* TODO: Should "memory" be in the clobber list here? */
}

inline static uint32_t peekl(uint16_t selector, void* offset)
{
    uint32_t ret;
    PH_ASM
    (
        "push %%fs\n\t"
        "mov %1, %%fs\n\t"
        "mov %%fs:(%2), %0\n\t"
        "pop %%fs"
        : "=r"(ret) : "g"(selector), "r"(offset)
    );
}

#endif // MEMORY_ACCESS_INTRINSICS_H