/*
 * Created by vitriol1744 on 24.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Utility/Logger.hpp"
#include "Utility/Stacktrace.hpp"

#define BIT(n)          (1ull << n)
#define PH_UNUSED       [[maybe_unused]]

#define PH_ARCH_X86_64  BIT(0)
#define PH_ARCH_AARCH64 BIT(1)

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

using symbol                   = void*[];

using InterruptHandlerFunction = void (*)(struct CPUContext*);

[[noreturn]]
PH_UNUSED inline static void halt(struct CPUContext* = nullptr)
{
#if PH_ARCH == PH_ARCH_X86_64
    while (true) __asm__ volatile("cli; hlt");
#elif PH_ARCH == PH_ARCH_AARCH64
    while (true) asm volatile("wfi");
#endif
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

    Stacktrace::Print(16);
    halt(nullptr);
}
#define Panic(...)   panic(std::format(__VA_ARGS__))

#define Assert(expr) AssertMsg(expr, #expr)
#define AssertMsg(expr, msg)                                                   \
    !(expr) ? Panic("Assertion Failed: {}, In File: {}, At Line: {}", msg,     \
                    __FILE__, __LINE__)                                        \
            : (void)0;
#define ToDo() AssertMsg(false, "Function is not implemented!")
