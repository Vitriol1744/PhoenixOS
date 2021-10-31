#ifndef MEMORY_ACCESS_H
#define MEMORY_ACCESS_H

#include "common.h"

inline static dword_t farpeekl(word_t selector, void* offset)
{
    dword_t ret;
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

static inline void farpokeb(word_t selector, void* offset, byte_t value)
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

inline static dword_t peekl(word_t selector, void* offset)
{
    dword_t ret;
    PH_ASM
    (
        "push %%fs\n\t"
        "mov %1, %%fs\n\t"
        "mov %%fs:(%2), %0\n\t"
        "pop %%fs"
        : "=r"(ret) : "g"(selector), "r"(offset)
    );
}

#endif // MEMORY_ACCESS_H