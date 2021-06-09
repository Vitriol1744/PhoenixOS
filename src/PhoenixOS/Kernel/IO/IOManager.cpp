#include "IOManager.hpp"

#include "Kernel/Common.hpp"

void IOManager::Initialize(stivale2_struct* bootloader_data)
{
    Serial::Initialize();
    Terminal::Initialize(bootloader_data);
}