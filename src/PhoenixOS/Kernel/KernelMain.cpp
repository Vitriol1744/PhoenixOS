#include "Utility/Logger.hpp"
#include "Kernel/IO/IOManager.hpp"

#include "stdlib.h"

 // Kernel Entry Point
extern "C" void KernelMain(stivale2_struct* bootloader_data) 
{
    IOManager::Initialize(bootloader_data);
    LOG_INFO("Number: ", 249);

    while (true) 
	{
        asm ("hlt");
    }

    LOG_INFO("Terminated!");
}