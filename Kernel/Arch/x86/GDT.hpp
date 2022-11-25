#pragma once

#include <stdint.h>

// We are currently using limine's gdt
static constexpr const uint32_t GDT_KERNEL_CODE_SELECTOR64 = 0x28;
static constexpr const uint32_t GDT_KERNEL_DATA_SELECTOR64 = 0x30;