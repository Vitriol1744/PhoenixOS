#include "Arch/Arch.hpp"

#include "Drivers/Terminal.hpp"
#include "Memory/PhysicalMemoryManager.hpp"

#include "Utility/Logger.hpp"

#include <stddef.h>
#include <stdint.h>

static void halt(void)
{
    for (;;) __asm__ volatile("hlt");
}

extern "C" void kernelStart()
{
    if (!BootInfo::Initialize()) halt();
    Logger::EnableE9Logging();
    Terminal::Initialize();
    Logger::EnableTerminalLogging();
    LogTrace("\nBooting PhoenixOS...\n");
    LogInfo("Bootloader: %s, Version: %s\n", BootInfo::GetBootloaderName(),
            BootInfo::GetBootloaderVersion());
    LogInfo("Kernel Physical Address: %#p\nKernel Virtual Address: %#p\n",
            BootInfo::GetKernelPhysicalAddress(),
            BootInfo::GetKernelVirtualAddress());
    LogInfo("Kernel Boot Time: %d\n", BootInfo::GetBootTime());

    // Arch::Initialize();
    PhysicalMemoryManager::Initialize();

    //__asm__ volatile("sti;");
    // LogTrace("\nHello, %#-04lliWaaorld!", 15);

    halt();
}