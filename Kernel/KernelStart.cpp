#include <stdint.h>
#include <stddef.h>

#include "Drivers/Terminal.hpp"
 
static void halt(void) 
{
    for (;;) __asm__ volatile("hlt");
}
 
extern "C" void kernelStart() 
{
    if (!BootInfo::Initialize()) halt();
    BootInfo::TerminalWrite("Hello World", 11);
    Terminal::Initialize();
    Terminal::ClearScreen(0x00ffff);
    BootInfo::TerminalWrite("Hello World", 11);
 
    halt();
}