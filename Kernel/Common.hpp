#pragma once

#include "Drivers/Terminal.hpp"

#include "Utility/Logger.hpp"

#define BIT(n) (1 << n)

struct StackFrame
{
    StackFrame* rbp;
    uintptr_t   rip;
};

inline static void stackTrace()
{
    StackFrame* stackFrame;
#if PH_ARCH == PH_ARCH_X86_64
    __asm__ volatile("mov %0, rbp" : "=r"(stackFrame));
#elif PH_ARCH == PH_ARCH_IA32
    __asm__ volatile("mov %0, ebp" : "=r"(stackFrame));
#endif
    LogFatal("Stack Trace: \n");
    while (stackFrame != nullptr)
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
    __asm__ volatile("cli; hlt");
}