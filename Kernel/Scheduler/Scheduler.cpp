/*
 * Created by vitriol1744 on 30.11.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "Scheduler.hpp"

#include "BootInfo.hpp"
#include "Common.hpp"

#include "Arch/x86/IDT.hpp"
#include "Arch/x86/LAPIC.hpp"

#include "Memory/PhysicalMemoryManager.hpp"
#include "Scheduler/Spinlock.hpp"

#define PAGE_SIZE PhysicalMemoryManager::GetPageSize()

static Vector<Process*>  processQueue  = {};
static Process*          kernelProcess    = nullptr;
static Spinlock          lock         = {};

ProcessID GetUniquePID()
{
    static ProcessID pid = 0;
    return pid++;
}
ThreadID GetUniqueTID()
{
    static ThreadID tid = 0;

    return tid++;
}

extern "C" void          switch_process(CPUContext* context);

void                     Schedule(CPUContext* context)
{
    //TODO: Implement some idle task when no threads are available for running
    lock.Lock();
    CPU*     cpu = CPU::GetCurrent();
    Process*  nextProcess;
    Thread*   nextThread;

    if (processQueue.GetSize() == 0) goto exit_schedule;
    nextProcess = processQueue[0];

    if (nextProcess->pid == -1) goto exit_schedule;

    if (nextProcess->threads.GetSize() == 0) goto exit_schedule;
    nextThread = nextProcess->threads[0];
    if (nextThread->tid == -1) goto exit_schedule;
    if (nextThread->state == ThreadState::eRunning) goto exit_schedule;

    if (cpu->runningThread)
    {
        auto lastProcess = cpu->runningThread->process;
        auto lastThread = cpu->runningThread;

        lastThread->state  = ThreadState::eIdle;

        processQueue.PushBack(lastProcess);

        if (cpu->runningThread) cpu->runningThread->context = *context;
        lastThread->context = *context;
    }
    nextProcess->threads.PopFront();
    nextProcess->threads.PushBack(nextThread);

    cpu->runningThread = nextThread;

    VirtualMemoryManager::SwitchPageMap(cpu->pageMap);

    nextThread->state  = ThreadState::eRunning;

    processQueue.PopFront();

    LAPIC::SendEOI();

    lock.Unlock();
    LAPIC::OneShot(5000, Schedule);
    switch_process(&nextThread->context);
exit_schedule:
    LAPIC::OneShot(5000, Schedule);
    lock.Unlock();
}

namespace Scheduler
{
    void Initialize()
    {
        kernelProcess = CreateProcess(VirtualMemoryManager::GetKernelPageMap());
        LogInfo("Scheduler: Kernel process created with pid %u", kernelProcess->pid);

        LogInfo("Scheduler: Initialized");
        LAPIC::OneShot(5000, Schedule);
    }

    __attribute__((noreturn)) void Await() { NotImplemented(); }
    __attribute__((noreturn)) void Yield()
    {
        __asm__ volatile("sti");
        LAPIC::OneShot(10000, Schedule);
        for (;;) __asm__ volatile("hlt");
    }

    Process* CreateProcess(PageMap& pageMap)
    {
        auto process = new Process;
        process->pid     = GetUniquePID();
        process->pageMap = pageMap;
        if (!pageMap.Exists()) LogError("Invalid PageMap!");

        lock.Lock();
        processQueue.PushBack(process);
        lock.Unlock();
        return process;
    }

    Thread* CreateThread(Process* process, uintptr_t rip, uint16_t cs, uint16_t ds)
    {
        if (process->pid == -1) return nullptr;

        const uintptr_t stack
            = (uintptr_t)PhysicalMemoryManager::CallocatePages(1) + PAGE_SIZE;

        auto thread          = new Thread;
        thread->process         = process;
        thread->tid             = GetUniqueTID();
        thread->state           = ThreadState::eIdle;
        thread->kernelStack     = stack + HIGHER_HALF_OFFSET;
        thread->kernelStackSize = PAGE_SIZE;
        thread->context.rip     = rip;
        thread->context.rsp     = thread->kernelStack;
        thread->context.cs      = KERNEL_CODE_SELECTOR;
        thread->context.ss      = KERNEL_DATA_SELECTOR;
        thread->context.flags = 0x202;

        lock.Lock();
        process->threads.PushBack(thread);
        lock.Unlock();

        return thread;
    }
    Thread*  CreateKernelThread(Process* process, uintptr_t rip)
    {
        return CreateThread(process, rip, KERNEL_CODE_SELECTOR, KERNEL_DATA_SELECTOR);
    }
    Thread*  CreateUserThread(Process* process, uintptr_t rip)
    {
        return CreateThread(process, rip, USERLAND_CODE_SELECTOR, USERLAND_DATA_SELECTOR);
    }

    Process* GetKernelProcess() { return kernelProcess; }
} // namespace Scheduler