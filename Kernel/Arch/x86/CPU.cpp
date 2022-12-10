#include "CPU.hpp"

Vector<CPU>      CPU::cpus    = {};

// TODO: this doesnt belong here
extern "C" void* __dso_handle = nullptr;
extern "C" void  __cxa_atexit() {}

#include "Arch/x86/IDT.hpp"
#include "Arch/x86/LAPIC.hpp"
#include "Scheduler/Spinlock.hpp"

#include "Scheduler/Scheduler.hpp"

#include "BootInfo.hpp"
#include "Common.hpp"
#include "Memory/PhysicalMemoryManager.hpp"
#include "Utility/KLibC.hpp"
#include "limine.h"

#include <stddef.h>

#define PAGE_SIZE PhysicalMemoryManager::GetPageSize()

static uint64_t                           onlineAPs  = 0;
static Spinlock                           lock       = {};

// TODO: This also doesn't belong here,
//  or does it?
static volatile struct limine_smp_request smpRequest = {
    .id       = LIMINE_SMP_REQUEST,
    .revision = 0,
};

void        Schedule(CPUContext*);
static void InitializeSingleCPU(struct limine_smp_info* smp_info)
{
    GDT::Load();
    IDT::Load();
    VirtualMemoryManager::SwitchPageMap(
        VirtualMemoryManager::GetKernelPageMap());

    CPU::SetGSBase((uintptr_t)smp_info->extra_argument);
    CPU::SetKernelGSBase((uintptr_t)smp_info->extra_argument);

    CPU* cpu = CPU::GetCurrent();

    GDT::LoadTSS(&cpu->tss);

    LAPIC::Initialize();
    LogInfo("CPU: %#016p", cpu);

    lock.Lock();
    ++onlineAPs;
    lock.Unlock();

    LogInfo("await");
    Scheduler::Yield();
}

void CPU::InitializeBSP()
{
    cpus.Resize(smpRequest.response->cpu_count);
    uint64_t stack;
    __asm__ volatile("mov %%rsp, %0" : "=r"(stack));

    cpus[0]            = {};
    cpus[0].id         = 0;
    cpus[0].lapicID    = smpRequest.response->bsp_lapic_id;
    cpus[0].tss.rsp[0] = stack;
    cpus[0].pageMap    = VirtualMemoryManager::GetKernelPageMap();

    CPU::SetGSBase((uintptr_t)&cpus[0]);
    CPU::SetKernelGSBase((uintptr_t)&cpus[0]);

    CPU* cpu = CPU::GetCurrent();
    GDT::LoadTSS(&cpu->tss);

    LAPIC::Initialize();
}

void CPU::StartUpAPs()
{
    struct limine_smp_response* smpResponse = smpRequest.response;
    Assert(smpResponse != NULL);

    LogInfo("Starting APs...", smpResponse->cpu_count);

    struct limine_smp_info** cpus     = smpResponse->cpus;
    uint32_t                 cpuIndex = 1;
    for (size_t i = 0; i < smpResponse->cpu_count; ++i)
    {
        if (cpus[i]->lapic_id == smpRequest.response->bsp_lapic_id) continue;
        const uint64_t stack
            = (uintptr_t)PhysicalMemoryManager::CallocatePages(1) + PAGE_SIZE
            + BootInfo::GetHHDMOffset();

        CPU::cpus[cpuIndex]            = {};
        CPU::cpus[cpuIndex].id         = i;
        CPU::cpus[cpuIndex].lapicID    = cpus[i]->lapic_id;
        CPU::cpus[cpuIndex].tss.rsp[0] = stack;
        CPU::cpus[cpuIndex].pageMap = VirtualMemoryManager::GetKernelPageMap();

        LogInfo("Index: %d", cpuIndex);
        cpus[i]->extra_argument = (uint64_t)&CPU::cpus[cpuIndex];
        cpuIndex++;
        cpus[i]->goto_address = InitializeSingleCPU;
    }

    while (onlineAPs != smpResponse->cpu_count - 1) __asm__ volatile("nop");
    LogInfo("APs Started!");
    LogInfo("Online APs count: %lld", onlineAPs);
}
