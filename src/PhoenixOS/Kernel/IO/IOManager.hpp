#pragma once

#include "Kernel/IO/Serial.hpp"
#include "Kernel/IO/ANSIEscapeCodes.hpp"
#include "Kernel/IO/Terminal.hpp"

class IOManager
{
    public:
        static void Initialize(stivale2_struct* bootloader_data);
};