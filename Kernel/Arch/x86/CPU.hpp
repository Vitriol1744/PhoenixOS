#pragma once

#include "Arch/x86/Types.hpp"

#include <assert.h>

namespace CPU
{
    inline static bool GetInterruptFlag()
    {
        uint64_t rflags;
        __asm__ volatile("pushf\n\tpop %0" : "=m"(rflags));

        return rflags & (1 << 9);
    }
} // namespace CPU