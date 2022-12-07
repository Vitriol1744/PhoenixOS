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

inline static void halt()
{
    while (true)
        ;
}

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

inline static void panic(const char* msg, ...)
{
    Terminal::ClearScreen(0x0000ff);
    LogFatal("Kernel Panic!\n");

    va_list args;
    va_start(args, msg);
    Logger::Logv(LogLevel::eFatal, msg, args);
    va_end(args);
    LogFatal("\n");

    stackTrace();
    halt();
}

#define Assert(expr) AssertMsg(expr, #expr)
#define AssertMsg(expr, msg)                                                   \
    if (expr) {}                                                               \
    else                                                                       \
    {                                                                          \
        panic("Assertion Failed: %s, In File: %s, At Line: %d", msg, __FILE__, \
              __LINE__);                                                       \
    }