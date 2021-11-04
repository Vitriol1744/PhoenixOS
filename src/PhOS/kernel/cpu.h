#ifndef CPU_H
#define CPU_H

#include "common.h"

static inline void cpuid(int32_t code, dword_t* a, dword_t* d)
{
    PH_ASM_VOL("cpuid" : "=a"(*a), "=d"(*d) : "0"(code) : "ebx", "ecx");
}

#endif // CPU_H