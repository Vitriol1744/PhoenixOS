#pragma once

#include "Arch/Arch.hpp"

#include "Common.hpp"

// We are currently using limine's gdt
inline static constexpr const uint64_t GDT_KERNEL_CODE_SELECTOR64 = 0x08;
inline static constexpr const uint64_t GDT_KERNEL_DATA_SELECTOR64 = 0x10;

namespace GDT
{
    void Initialize();
    void Load();
}