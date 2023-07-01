/*
 * Created by vitriol1744 on 24.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include <stddef.h>
#include <stdint.h>

static constexpr const uint32_t TERMINAL_COLOR_BLACK = 0x000000;

class Terminal
{
  public:
    static bool     Initialize();

    static void     ClearScreen(uint32_t color = TERMINAL_COLOR_BLACK);
    static void     PutChar(uint64_t c);
    static void     PrintString(const char* string, size_t length);
    static void     PrintString(const char* string);
    static void     ScrollDown(uint8_t lines = 1);

    static void     SetColor(uint64_t color);
    static void     SetForegroundColor(uint32_t color);
    static void     SetBackgroundColor(uint32_t color);

    static uint64_t GetColor();
    static uint32_t GetForegroundColor();
    static uint32_t GetBackgroundColor();

  private:
    Terminal() = default;
};