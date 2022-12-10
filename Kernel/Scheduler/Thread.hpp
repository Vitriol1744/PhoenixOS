#pragma once

#include "Arch/x86/CPU.hpp"

using ThreadID = int64_t;

enum class ThreadState
{
    eIdle,
    eRunning,
};

struct Thread
{
    struct Process* process;
    ThreadID    tid;
    ThreadState state;

    uintptr_t   kernelStack;
    uintptr_t   kernelStackSize;

    CPUContext  context;
};