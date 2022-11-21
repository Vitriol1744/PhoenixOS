#pragma once

#include "BootInfo.hpp"

static constexpr const uint32_t TERMINAL_COLOR_BLACK = 0xffffff;

class Terminal
{
    public:
        static bool Initialize();

        static void ClearScreen(uint32_t color = TERMINAL_COLOR_BLACK);

    private:
        static uint32_t x;
        static uint32_t y;

        Terminal() = delete;
};