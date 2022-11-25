#pragma once

#include <stdint.h>

static constexpr const uint32_t FRAMEBUFFER_MEMORY_MODEL_RGB = 0x01;

struct Framebuffer
{
    uintptr_t base;
    uint64_t  width;
    uint64_t  height;
    uint64_t  pitch;
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

namespace BootInfo
{
    bool         Initialize();
    void         TerminalWrite(const char* str, const uint64_t length);
    Framebuffer* GetFramebuffer();
    const char*  GetBootloaderName();
    const char*  GetBootloaderVersion();
    uint64_t     GetBootTime();
    uint64_t     GetKernelPhysicalAddress();
    uint64_t     GetKernelVirtualAddress();

}; // namespace BootInfo