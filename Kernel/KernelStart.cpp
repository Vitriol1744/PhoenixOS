/*
 * Created by vitriol1744 on 22.11.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "ACPI.hpp"
#include "Common.hpp"

#include "Arch/x86/HPET.hpp"
#include "Arch/x86/IDT.hpp"
#include "Arch/x86/PIC.hpp"
#include "Arch/x86/IO.hpp"

#include "Scheduler/Scheduler.hpp"

#include "Memory/PhysicalMemoryManager.hpp"

[[noreturn]] static void kernelInitialize();
[[noreturn]] static void thread2();

using ConstructorFunction = void (*)();

extern ConstructorFunction __init_array_start[];
extern ConstructorFunction __init_array_end[];

#define QemuExit() IO::Out<byte>(0x501, 0x31)

void syscall(CPUContext* context)
{
    LogError((const char*)context->rdi);
}

extern "C" __attribute__((noreturn)) void kernelStart()
{
    // Terminal::ClearScreen(0x383c3c);

    LogTrace("Booting PhoenixOS...\n");

    LogInfo("Bootloader: %s, Version: %s\n", BootInfo::GetBootloaderName(),
            BootInfo::GetBootloaderVersion());
    LogInfo("HHDM Offset: %llx\n", BootInfo::GetHHDMOffset());
    LogInfo("Kernel Physical Address: %#p\nKernel Virtual Address: %#p\n ",
            BootInfo::GetKernelPhysicalAddress(),
            BootInfo::GetKernelVirtualAddress());
    LogInfo("Kernel Boot Time: %d\n", BootInfo::GetBootTime());

    GDT::Initialize();
    GDT::Load();
    PIC::Remap(0x20, 0x28);

    IDT::Initialize();
    IDT::Load();

    PhysicalMemoryManager::Initialize();

    for (ConstructorFunction* entry = __init_array_start;
         entry < __init_array_end; entry++)
    {
        ConstructorFunction constructor = *entry;
        constructor();
    }
    VirtualMemoryManager::Initialize();

    ACPI::Initialize();
    HPET::Initialize();

    LogInfo("\n ____  _                      _       ___  ____  \n");
    LogInfo("|  _ \\| |__   ___   ___ _ __ (_)_  __/ _ \\/ ___| \n");
    LogInfo("| |_) | '_ \\ / _ \\ / _ \\ '_ \\| \\ \\/ / | |\\___ \\ \n");
    LogInfo("|  __/| | | | (_) |  __/ | | | |>  <| |_| |___)|\n");
    LogInfo("|_|   |_| |_|\\___/ \\___|_| |_|_/_/\\_\\___/|____/ \n");

    CPU::InitializeBSP();
    CPU::StartUpAPs();

    IDT::RegisterInterruptHandler(0x80, syscall, DPL_RING3);

    Scheduler::Initialize();

    Scheduler::CreateKernelThread(Scheduler::GetKernelProcess(), reinterpret_cast<uintptr_t>(kernelInitialize));

    auto p
        = Scheduler::CreateProcess(VirtualMemoryManager::GetKernelPageMap());
    Scheduler::CreateUserThread(p, (uintptr_t)thread2);

    Scheduler::Yield();

    QemuExit();
}

#if 0
    #define LogError(...)
    #define LogWarn(...)
#endif

[[noreturn]] static void kernelInitialize()
{
    while (true) LogInfo("W");
}
[[noreturn]] static void thread2()
{
    const char* str = "Hello, World!";
    __asm__ volatile("mov %0, %%rdi; int $0x80" : : "r"(str));

    while (true)
    {
        __asm__ volatile("mov %0, %%rdi; int $0x80" : : "r"(str));
        __asm__ volatile("nop");
    }
}
