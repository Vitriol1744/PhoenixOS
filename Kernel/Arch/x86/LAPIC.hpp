#pragma once

#include "Common.hpp"

#include "Arch/x86/IDT.hpp"

inline static constexpr const uint32_t LAPIC_TIMER_ISR = 0x20;

namespace LAPIC
{
    void    Initialize();
    void    SendEOI();
    uint8_t GetCurrentCPU();
    void    OneShot(uint64_t us, InterruptHandler handler);
}; // namespace LAPIC
