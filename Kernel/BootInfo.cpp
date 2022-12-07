#include "BootInfo.hpp"

#include "Common.hpp"

#include "limine.h"

#include <stdatomic.h>

static constexpr const uint32_t                DEFAULT_STACK_SIZE    = 65536;

static volatile limine_bootloader_info_request bootloaderInfoRequest = {
    .id       = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0,
};
static volatile limine_stack_size_request stackSizeRequest = {
    .id         = LIMINE_STACK_SIZE_REQUEST,
    .revision   = 0,
    .stack_size = DEFAULT_STACK_SIZE,
};
static volatile limine_hhdm_request hhdmRequest = {
    .id       = LIMINE_HHDM_REQUEST,
    .revision = 0,
};
static volatile limine_framebuffer_request framebufferRequest = {
    .id       = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
};
#ifdef PH_USE_PML5
static volatile limine_5_level_paging_request pml5Request = {
    .id       = LIMINE_5_LEVEL_PAGING_REQUEST,
    .revision = 0,
};
#endif
/*static volatile limine_smp_request smpRequest = {
    .id       = LIMINE_SMP_REQUEST,
    .revision = 0,
};*/
static volatile limine_memmap_request memmapRequest = {
    .id       = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
};

static volatile limine_entry_point_request entryPointRequest = {
    .id       = LIMINE_ENTRY_POINT_REQUEST,
    .revision = 0,
    .entry    = BootInfo::Initialize,
};
static volatile limine_kernel_file_request kernelFileRequest = {
    .id       = LIMINE_KERNEL_FILE_REQUEST,
    .revision = 0,
};
static volatile limine_rsdp_request rsdpRequest = {
    .id       = LIMINE_RSDP_REQUEST,
    .revision = 0,
};
static volatile limine_boot_time_request bootTimeRequest = {
    .id       = LIMINE_BOOT_TIME_REQUEST,
    .revision = 0,
};
static volatile limine_kernel_address_request kernelAddressRequest = {
    .id       = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0,
};

static Framebuffer      framebuffer         = {};
static MemoryMapEntry** memoryMap           = nullptr;
static uint64_t         memoryMapEntryCount = 0;

namespace BootInfo
{
    extern "C" void Initialize()
    {
        (void)stackSizeRequest.response;
        (void)entryPointRequest.response;
#ifdef PH_USE_PML5
        (void)pml5Request.response;
#endif

        Logger::EnableE9Logging();
        if (!framebufferRequest.response
            || framebufferRequest.response->framebuffer_count < 1)
            panic(__FILE__ "Boot: Failed to acquire the framebuffer!\n");
        limine_framebuffer* limineFramebuffer
            = framebufferRequest.response->framebuffers[0];
        framebuffer.base
            = reinterpret_cast<uintptr_t>(limineFramebuffer->address);
        framebuffer.width        = limineFramebuffer->width;
        framebuffer.height       = limineFramebuffer->height;
        framebuffer.pitch        = limineFramebuffer->pitch;
        framebuffer.bitsPerPixel = limineFramebuffer->bpp;
        framebuffer.memoryModel
            = limineFramebuffer->memory_model == LIMINE_FRAMEBUFFER_RGB
                ? FRAMEBUFFER_MEMORY_MODEL_RGB
                : 0;
        framebuffer.redMaskSize    = limineFramebuffer->red_mask_size;
        framebuffer.redMaskShift   = limineFramebuffer->red_mask_shift;
        framebuffer.greenMaskSize  = limineFramebuffer->green_mask_size;
        framebuffer.greenMaskShift = limineFramebuffer->green_mask_shift;
        framebuffer.blueMaskSize   = limineFramebuffer->blue_mask_size;
        framebuffer.blueMaskShift  = limineFramebuffer->blue_mask_shift;

        Terminal::Initialize();
        Logger::EnableTerminalLogging();

        if (!memmapRequest.response || memmapRequest.response->entry_count == 0)
            panic("Failed to acquire limine memory map entries!");

        memoryMap           = (MemoryMapEntry**)memmapRequest.response->entries;
        memoryMapEntryCount = memmapRequest.response->entry_count;
        LogInfo("Framebuffer Address: %#p\n", framebuffer.base);
        __asm__ goto("jmp kernelStart");
    }
    uint64_t     GetHHDMOffset() { return hhdmRequest.response->offset; }
    Framebuffer* GetFramebuffer() { return &framebuffer; }
    const char*  GetBootloaderName()
    {
        return bootloaderInfoRequest.response->name;
    }
    const char* GetBootloaderVersion()
    {
        return bootloaderInfoRequest.response->version;
    }
    uintptr_t GetRSDPAddress()
    {
        return reinterpret_cast<uintptr_t>(rsdpRequest.response->address);
    }
    uint64_t GetBootTime() { return bootTimeRequest.response->boot_time; }
    uint64_t GetKernelPhysicalAddress()
    {
        return kernelAddressRequest.response->physical_base;
    }
    uint64_t GetKernelVirtualAddress()
    {
        return kernelAddressRequest.response->virtual_base;
    }

    MemoryMap GetMemoryMap(uint64_t& entryCount)
    {
        entryCount = memoryMapEntryCount;
        return memoryMap;
    }

} // namespace BootInfo