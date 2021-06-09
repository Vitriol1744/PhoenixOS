#pragma once

#include <stddef.h>
#include <string.h>
#include "stdlib.h"

#include "Kernel/Stivale.hpp"
#include "Kernel/IO/ANSIEscapeCodes.hpp"

using print_func = void (*)(const char* string, size_t length);

class Terminal
{
    public:
        enum class Color
        {
            NONE,
            BLACK,
            RED,
            GREEN,
            YELLOW,
            BLUE,
            MAGENTA,
            CYAN,
            WHITE
        };

        static bool Initialize(stivale2_struct* bootloader_data);

        inline static void Print(char msg) { Print(&msg, 1); }
        inline static void Print(const char* msg) { Print(msg, strlen(msg)); }
        inline static void Print(long long msg) 
        { 
            Print("Print"); char buffer[20]; Print(itoa(msg, buffer, 10)); 
        }

        static void Print(const char* msg, size_t length);
        static void SetColor(Terminal::Color foreground_color, Terminal::Color background_color);

    private:
        static print_func print;
};

using Color = Terminal::Color;