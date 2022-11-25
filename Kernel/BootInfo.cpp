#include "BootInfo.hpp"

#include "limine.h"

static constexpr const uint32_t DEFAULT_STACK_SIZE = 65536;

void terminalCallback(struct limine_terminal* terminal, uint64_t, uint64_t,
                      uint64_t, uint64_t)
{
    // TODO: terminalCallback
    return;
}

static volatile struct limine_bootloader_info_request bootloaderInfoRequest = {
    .id       = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0,
};
static volatile struct limine_stack_size_request stackSizeRequest = {
    .id         = LIMINE_STACK_SIZE_REQUEST,
    .revision   = 0,
    .stack_size = DEFAULT_STACK_SIZE,
};
static volatile struct limine_hhdm_request hhdmRequest = {
    .id       = LIMINE_HHDM_REQUEST,
    .revision = 0,
};
static volatile struct limine_terminal_request terminalRequest = {
    .id       = LIMINE_TERMINAL_REQUEST,
    .revision = 0,
    .callback = terminalCallback,
};
static volatile struct limine_framebuffer_request framebufferRequest = {
    .id       = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
};
#ifdef PH_USE_PML5
static volatile struct limine_5_level_paging_request pml5Request = {
    .id       = LIMINE_5_LEVEL_PAGING_REQUEST,
    .revision = 0,
};
#endif
// TODO: SMP
static volatile struct limine_memmap_request memmapRequest = {
    .id       = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
};
static volatile struct limine_kernel_file_request kernelFileRequest = {
    .id       = LIMINE_KERNEL_FILE_REQUEST,
    .revision = 0,
};
static volatile struct limine_rsdp_request rsdpRequest = {
    .id       = LIMINE_RSDP_REQUEST,
    .revision = 0,
};
static volatile struct limine_boot_time_request bootTimeRequest = {
    .id       = LIMINE_BOOT_TIME_REQUEST,
    .revision = 0,
};
static volatile struct limine_kernel_address_request kernelAddressRequest = {
    .id       = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0,
};

static Framebuffer framebuffer = {};
namespace BootInfo
{
    bool Initialize()
    {
        if (!terminalRequest.response
            || terminalRequest.response->terminal_count < 1)
            return false;
        if (!framebufferRequest.response
            || framebufferRequest.response->framebuffer_count < 1)
            return false;

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

        return true;
    }
    void TerminalWrite(const char* str, const uint64_t length)
    {
        terminalRequest.response->write(terminalRequest.response->terminals[0],
                                        str, length);
    }
    Framebuffer* GetFramebuffer() { return &framebuffer; }
    const char*  GetBootloaderName()
    {
        return bootloaderInfoRequest.response->name;
    }
    const char* GetBootloaderVersion()
    {
        return bootloaderInfoRequest.response->version;
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

} // namespace BootInfo