#pragma once

#include <stdint.h>

namespace HPET
{
    void Initialize();
    void Sleep(uint64_t milliseconds);
} // namespace HPET