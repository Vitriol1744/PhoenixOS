/*
 * Created by vitriol1744 on 26.11.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Arch/x86/Types.hpp"

#include "Arch/x86/GDT.hpp"

#include "Memory/VirtualMemoryManager.hpp"

#include "Utility/Vector.hpp"

struct CPUContext
{
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;

    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    uint64_t interruptVector;
    uint64_t errorCode;

    uint64_t rip;
    uint64_t cs;
    uint64_t flags;
    uint64_t rsp;
    uint64_t ss;
};

#include "Arch/x86/CPUID.hpp"

using ProcessID = int64_t;
using ThreadID  = int64_t;

class CPU
{
  public:
    CPU() { self = this; }

    static void        InitializeBSP();
    static void        StartUpAPs();

    inline static void ID(uint64_t code, uint64_t& rax, uint64_t& rbx,
                          uint64_t& rcx, uint64_t& rdx)
    {
        __asm__ volatile("cpuid"
                         : "=a"(rax), "=b"(rbx), "=c"(rcx), "=d"(rdx)
                         : "a"(code));
    }
    template <bool flag>
    __attribute__((always_inline)) inline static void SetInterruptFlag()
    {
        if constexpr (flag) __asm__ volatile("sti");
        else __asm__ volatile("cli");
    }
    __attribute__((always_inline)) inline static void
    SetInterruptFlag(bool flag)
    {
        if (flag) __asm__ volatile("sti");
        else __asm__ volatile("cli");
    }
    inline static bool GetInterruptFlag()
    {
        uint64_t rflags;
        __asm__ volatile("pushf\n\tpop %0" : "=m"(rflags));

        return rflags & (1 << 9);
    }

    inline static void WriteMSR(uint32_t msr, uint64_t value)
    {
        const uint64_t rdx = value >> 32;
        const uint64_t rax = value;
        __asm__ volatile("wrmsr" : : "a"(rax), "d"(rdx), "c"(msr) : "memory");
    }
    inline static uint64_t ReadMSR(uint32_t msr)
    {
        uint64_t rdx = 0;
        uint64_t rax = 0;
        __asm__ volatile("rdmsr" : "=a"(rax), "=d"(rdx) : "c"(msr) : "memory");
        return (rdx << 32) | rax;
    }

    inline static void SetGSBase(uintptr_t address)
    {
        WriteMSR(0xc0000101, address);
    }
    inline static void SetKernelGSBase(uintptr_t address)
    {
        WriteMSR(0xc0000102, address);
    }
    inline static Vector<CPU>& GetCPUs() { return cpus; }
    inline static CPU*     GetCurrent() { return (CPU*)ReadMSR(0xc0000101); }
    inline static uint64_t GetCurrentID()
    {
        uint64_t cpuID = 0;
        __asm__ volatile("mov %%gs:%a[off], %[val]"
                         : [val] "=r"(cpuID)
                         : [off] "ir"(offsetof(CPU, id)));
        return cpuID;
    }
    inline uint64_t GetLapicFrequency() { return lapicFrequency; }
    inline void     SetLapicFrequency(uint64_t freq) { lapicFrequency = freq; }

    static Vector<CPU> cpus;
    CPU*               self = nullptr;
    uint64_t           lapicFrequency = 0;
    uint64_t           id = 0;
    uint32_t           lapicID = 0;

    struct Thread* runningThread = nullptr;

    TaskStateSegment   tss = {};
    PageMap            pageMap = {};
};