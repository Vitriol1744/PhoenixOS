#include "Arch/x86/IDT.hpp"

#include "Drivers/Terminal.hpp"
#include "Utility/Logger.hpp"

#include <stddef.h>
#include <stdint.h>

static void halt(void)
{
    for (;;) __asm__ volatile("hlt");
}

static IDT      idt;
extern "C" void kernelStart()
{
    if (!BootInfo::Initialize()) halt();
    Logger::EnableE9Logging();
    Terminal::Initialize();
    Logger::EnableTerminalLogging();
    LogTrace("\nBooting PhoenixOS...\n");
    LogInfo("Bootloader: %s, Version: %s\n", BootInfo::GetBootloaderName(),
            BootInfo::GetBootloaderVersion());
    LogInfo("Kernel Physical Address: %p\nKernel Virtual Address: %p\n",
            BootInfo::GetKernelPhysicalAddress(),
            BootInfo::GetKernelVirtualAddress());
    LogInfo("Kernel Boot Time: %d\n", BootInfo::GetBootTime());

    idt.Initialize();
    IDT::Load(&idt);
    __asm__ volatile("int 0x10");
    LogTrace("\nHello, %#-04lliWorld!", 15);

    halt();
}