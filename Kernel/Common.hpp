/*
 * Created by vitriol1744 on 24.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Utility/Logger.hpp"

#define BIT(n)    (1ull << n)
#define PH_UNUSED [[maybe_unused]]

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

using symbol                   = void*[];

using InterruptHandlerFunction = void (*)(struct CPUContext*);

[[noreturn]]
PH_UNUSED inline static void halt(struct CPUContext* = nullptr)
{
    while (true) __asm__ volatile("cli; hlt");
}

#define ENABLE_CLEAR_SCREEN_ON_PANIC false
[[noreturn]]
inline void panic(std::string_view msg)
{
    // TODO: Stop all cpus
#if ENABLE_CLEAR_SCREEN_ON_PANIC == true
    Terminal::ClearScreen(0x0000ff);
#endif
    Logger::LogChar(BACKGROUND_COLOR_RED);
    Logger::LogChar(FOREGROUND_COLOR_WHITE);
    Logger::LogString("Kernel Panic!\n\r");
    Logger::LogString(msg);
    Logger::LogChar(RESET_COLOR);
    Logger::LogString("\n\r");

    // TODO: Print stacktrace
    halt(nullptr);
}
#define Panic(...)   panic(std::format(__VA_ARGS__))

#define Assert(expr) AssertMsg(expr, #expr)
#define AssertMsg(expr, msg)                                                   \
    !(expr) ? Panic("Assertion Failed: %s, In File: %s, At Line: %d", msg,     \
                    __FILE__, __LINE__)                                        \
            : (void)0;
#define ToDo() AssertMsg(false, "Function is not implemented!")
