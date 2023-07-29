/*
 * Created by vitriol1744 on 24.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "limine.h"

static constexpr const uint32_t FRAMEBUFFER_MEMORY_MODEL_RGB
    = LIMINE_FRAMEBUFFER_RGB;

struct Framebuffer
{
    uintptr_t base;
    uint32_t  width;
    uint32_t  height;
    uint32_t  pitch;
    uint16_t  bitsPerPixel;
    uint8_t   memoryModel;
    uint8_t   redMaskSize;
    uint8_t   redMaskShift;
    uint8_t   greenMaskSize;
    uint8_t   greenMaskShift;
    uint8_t   blueMaskSize;
    uint8_t   blueMaskShift;

    void      PutPixel(uint32_t pixel, uint32_t x, uint32_t y)
    {
        // TODO: Don't assume pixel format
        uint32_t* pixelOffset
            = (uint32_t*)(base + y * pitch + (x * (bitsPerPixel / 8)));
        // TODO: We shouldn't change unused bits
        *pixelOffset = pixel;
    }
};

inline static constexpr const uint32_t MEMORY_MAP_USABLE = LIMINE_MEMMAP_USABLE;
inline static constexpr const uint32_t MEMORY_MAP_RESERVED
    = LIMINE_MEMMAP_RESERVED;
inline static constexpr const uint32_t MEMORY_MAP_ACPI_RECLAIMABLE
    = LIMINE_MEMMAP_ACPI_RECLAIMABLE;
inline static constexpr const uint32_t MEMORY_MAP_ACPI_NVS
    = LIMINE_MEMMAP_ACPI_NVS;
inline static constexpr const uint32_t MEMORY_MAP_BAD_MEMORY
    = LIMINE_MEMMAP_BAD_MEMORY;
inline static constexpr const uint32_t MEMORY_MAP_BOOTLOADER_RECLAIMABLE
    = LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE;
inline static constexpr const uint32_t MEMORY_MAP_KERNEL_AND_MODULES
    = LIMINE_MEMMAP_KERNEL_AND_MODULES;
inline static constexpr const uint32_t MEMORY_MAP_FRAMEBUFFER
    = LIMINE_MEMMAP_FRAMEBUFFER;

using MemoryMapEntry = limine_memmap_entry;
using MemoryMap      = MemoryMapEntry**;

namespace BootInfo
{
    const char*          GetBootloaderName();
    const char*          GetBootloaderVersion();
    uint64_t             GetHHDMOffset();
    Framebuffer*         GetFramebuffer();
    limine_smp_response* GetSMP_Response();
    MemoryMap            GetMemoryMap(uint64_t& entryCount);
    limine_file*         FindModule(const char* name);
    uintptr_t            GetRSDPAddress();
    uint64_t             GetBootTime();
    uint64_t             GetKernelPhysicalAddress();
    uint64_t             GetKernelVirtualAddress();
}; // namespace BootInfo