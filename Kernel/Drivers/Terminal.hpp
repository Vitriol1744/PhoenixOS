#pragma once

#include "BootInfo.hpp"

#include <stddef.h>

static constexpr const uint32_t TERMINAL_COLOR_BLACK = 0x000000;

class Terminal
{
    public:
        static bool        Initialize();

        static void        ClearScreen(uint32_t color = TERMINAL_COLOR_BLACK);
        static void        PutChar(uint32_t c);
        inline static void PrintString(const char* string, size_t length)
        {
            while (length > 0)
            {
                PutChar(*string++);
                --length;
            }
        }
        inline static void PrintString(const char* string)
        {
            while (*string != '\0') PutChar(*string++);
        }

        static void     ScrollDown(uint8_t lines = 1);

        static void     SetForegroundColor(uint32_t color);
        static void     SetBackgroundColor(uint32_t color);

        static uint32_t GetForegroundColor();
        static uint32_t GetBackgroundColor();

    private:
        Terminal() = default;
};