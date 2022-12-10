#pragma once

#include "Common.hpp"

inline static constexpr const uint32_t PIT_BASE_FREQUENCY = 1193182;

namespace PIT
{
    void     Initialize();
    void     SetFrequency(size_t frequency);
    void     SetReloadValue(uint16_t reloadValue);
    uint64_t GetCurrentCount();
} // namespace PIT