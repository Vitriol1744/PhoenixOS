#pragma once

#include "Scheduler/Process.hpp"
#include "Scheduler/Thread.hpp"

namespace Scheduler
{
    void                           Initialize();

    __attribute__((noreturn)) void Await();
    __attribute__((noreturn)) void Yield();

    Process*                      CreateProcess(PageMap& page_map);
    ThreadID  CreateThread(Process* process, uintptr_t rip, uint16_t cs);

    Process* GetKernelProcess();
} // namespace Scheduler
