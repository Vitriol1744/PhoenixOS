#pragma once

#include "Common.hpp"

static constexpr const uint32_t FRAMEBUFFER_MEMORY_MODEL_RGB = 0x01;

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
        // We shouldn't change unused bits
        //*pixelOffset = (*pixelOffset & 0xff000000) | pixel;
        *pixelOffset = pixel;
    }
};

inline static constexpr const uint32_t MEMORY_MAP_USABLE                 = 0;
inline static constexpr const uint32_t MEMORY_MAP_RESERVED               = 1;
inline static constexpr const uint32_t MEMORY_MAP_ACPI_RECLAIMABLE       = 2;
inline static constexpr const uint32_t MEMORY_MAP_ACPI_NVS               = 3;
inline static constexpr const uint32_t MEMORY_MAP_BAD_MEMORY             = 4;
inline static constexpr const uint32_t MEMORY_MAP_BOOTLOADER_RECLAIMABLE = 5;
inline static constexpr const uint32_t MEMORY_MAP_KERNEL_AND_MODULES     = 6;
inline static constexpr const uint32_t MEMORY_MAP_FRAMEBUFFER            = 7;

struct MemoryMapEntry
{
    uint64_t base;
    uint64_t length;
    uint64_t type;
};

using MemoryMap = MemoryMapEntry**;

struct CPUInfo
{
    uint32_t  cpuID;
    uint32_t  lapicID;
    uint64_t  reserved;
    uintptr_t gotoAddress;
    uint64_t  extraArgument;
};

namespace BootInfo
{
    extern "C" void Initialize();

    const char*     GetBootloaderName();
    const char*     GetBootloaderVersion();
    uint64_t        GetHHDMOffset();
    Framebuffer*    GetFramebuffer();
    MemoryMap       GetMemoryMap(uint64_t& entryCount);
    CPUInfo**       GetCPUInfos(uint32_t& cpuCount);
    uintptr_t       GetRSDPAddress();
    uint64_t        GetBootTime();
    uint64_t        GetKernelPhysicalAddress();
    uint64_t        GetKernelVirtualAddress();
}; // namespace BootInfo