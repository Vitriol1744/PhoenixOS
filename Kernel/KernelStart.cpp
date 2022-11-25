#include <stddef.h>
#include <stdint.h>

#include "Drivers/Terminal.hpp"
#include "Utility/Logger.hpp"

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
    LogInfo("Bootloader: %s, Version: %s\n", BootInfo::GetBootloaderName(),
            BootInfo::GetBootloaderVersion());

    // TODO: Use printf when we actually implement it
    LogTrace("Booting PhoenixOS...\n");
    LogTrace("\nHello, %#lliWorld!", 15);

    // TODO: Print Boot Time
    // TODO: Print Kernel Physical and Virtual addresses

    halt();
}