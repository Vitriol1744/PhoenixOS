/*
 * Created by vitriol1744 on 25.11.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Drivers/Terminal.hpp"

#include "Utility/Logger.hpp"

#define BIT(n) (1ull << n)

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

using symbol = void*[];

struct StackFrame
{
    StackFrame* rbp;
    uintptr_t   rip;
};

using InterruptHandler = void (*)(class CPUContext*);
namespace IDT
{
    void RegisterInterruptHandler(uint32_t vector, InterruptHandler handler,
                                  uint8_t dpl);
}

inline static void halt(class CPUContext*) { __asm__ volatile("cli; hlt"); }

inline static void stackTrace()
{
    StackFrame* stackFrame;
#if PH_ARCH == PH_ARCH_X86_64
    __asm__ volatile("mov %%rbp, %0" : "=r"(stackFrame));
#elif PH_ARCH == PH_ARCH_IA32
    __asm__ volatile("mov %0, ebp" : "=r"(stackFrame));
#endif
    LogFatal("Stack Trace: \n");
    while (stackFrame != nullptr && stackFrame->rip != 0)
    {
        // TODO: Figure out a way to resolve function names
        LogFatal("%#p\n", stackFrame->rip);
        stackFrame = stackFrame->rbp;
    }
}

#define ENABLE_CLEAR_SCREEN_ON_PANIC false
[[noreturn]] inline static void panic(const char* msg, ...)
{
    // TODO: Stop all cpus
#if ENABLE_CLEAR_SCREEN_ON_PANIC != false
    Terminal::ClearScreen(0x0000ff);
#endif
    LogFatal("Kernel Panic!\n");

    va_list args;
    va_start(args, msg);
    Logger::Logv(LogLevel::eFatal, msg, args);
    va_end(args);
    LogFatal("\n");

    stackTrace();
    IDT::RegisterInterruptHandler(0x20, halt, 0);
    __asm__ volatile("cli; hlt");
}

#define Assert(expr) AssertMsg(expr, #expr)
#define AssertMsg(expr, msg)                                                   \
    if (expr) {}                                                               \
    else                                                                       \
    {                                                                          \
        panic("Assertion Failed: %s, In File: %s, At Line: %d", msg, __FILE__, \
              __LINE__);                                                       \
    }
#define NotImplemented() AssertMsg(false, "Function is not implemented!")