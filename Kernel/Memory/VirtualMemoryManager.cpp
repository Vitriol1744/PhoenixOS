/*
 * Created by vitriol1744 on 28.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "VirtualMemoryManager.hpp"

#include "Utility/Math.hpp"

extern "C" symbol text_start_addr;
extern "C" symbol text_end_addr;
extern "C" symbol rodata_start_addr;
extern "C" symbol rodata_end_addr;
extern "C" symbol data_start_addr;
extern "C" symbol data_end_addr;

#define PTE_ADDR_MASK       0x000ffffffffff000
#define PTE_GET_ADDR(value) ((value)&PTE_ADDR_MASK)

static PhysAddr* GetNextLevel(uint64_t* level, uint64_t nextLevelIndex)
{
    if ((level[nextLevelIndex] & PTE_PRESENT) != 0)
        return ToHigherHalfAddress<PhysAddr*>(
            PTE_GET_ADDR(level[nextLevelIndex]));

    auto nextLevel        = reinterpret_cast<PhysAddr*>(PMM::CallocatePages(1));

    level[nextLevelIndex] = (uint64_t)nextLevel | (uint64_t)PTE_PRESENT
                          | (uint64_t)PTE_WRITABLE | PTE_USER_ACCESSIBLE;

    return ToHigherHalfAddress<PhysAddr*>(
        reinterpret_cast<uintptr_t>(nextLevel));
}

static PageMap kernelPageMap;

void           PageMap::MapPage(VirtAddr virtAddr, PhysAddr physAddr,
                                uint64_t flags) const
{
    size_t pml5Entry = (virtAddr >> 48) & 0x1ffull;
    size_t pml4Entry = (virtAddr >> 39) & 0x1ffull;
    size_t pml3Entry = (virtAddr >> 30) & 0x1ffull;
    size_t pml2Entry = (virtAddr >> 21) & 0x1ffull;
    size_t pml1Entry = (virtAddr >> 12) & 0x1ffull;

    auto   pml4      = reinterpret_cast<PhysAddr*>(GetTopLevel());
    auto   pml3      = GetNextLevel(pml4, pml4Entry);
    auto   pml2      = GetNextLevel(pml3, pml3Entry);
    auto   pml1      = GetNextLevel(pml2, pml2Entry);

    // FIXME: Not every page should be user accessible
    pml1[pml1Entry]  = physAddr | flags | PTE_USER_ACCESSIBLE;
}

namespace VirtualMemoryManager
{
    void Initialize()
    {
        kernelPageMap.Create();
        Assert(kernelPageMap.GetTopLevel() != 0);

        for (size_t i = 256; i < 512; i++)
            Assert(GetNextLevel((PhysAddr*)kernelPageMap.GetTopLevel(), i)
                   != nullptr);

        for (uintptr_t addr = 0x1000; addr < 0x100000000; addr += PAGE_SIZE)
        {
            kernelPageMap.MapPage(addr, addr, PTE_PRESENT | PTE_WRITABLE);
            kernelPageMap.MapPage(ToHigherHalfAddress<uintptr_t>(addr), addr,
                                  PTE_PRESENT | PTE_WRITABLE);
        }

        for (auto p = Math::AlignDown(
                 reinterpret_cast<uintptr_t>(text_start_addr), PAGE_SIZE);
             p < Math::AlignUp(reinterpret_cast<uintptr_t>(text_end_addr),
                               PAGE_SIZE);
             p += PAGE_SIZE)
        {
            uintptr_t phys = p - BootInfo::GetKernelVirtualAddress()
                           + BootInfo::GetKernelPhysicalAddress();

            kernelPageMap.MapPage(p, phys, PTE_PRESENT | PTE_USER_ACCESSIBLE);
        }

        for (auto p = Math::AlignDown(
                 reinterpret_cast<uintptr_t>(rodata_start_addr), PAGE_SIZE);
             p < Math::AlignUp(reinterpret_cast<uintptr_t>(rodata_end_addr),
                               PAGE_SIZE);
             p += PAGE_SIZE)
        {
            uintptr_t phys = p - BootInfo::GetKernelVirtualAddress()
                           + BootInfo::GetKernelPhysicalAddress();
            kernelPageMap.MapPage(p, phys, PTE_PRESENT);
        }

        for (auto p = Math::AlignDown(
                 reinterpret_cast<uintptr_t>(data_start_addr), PAGE_SIZE);
             p < Math::AlignUp(reinterpret_cast<uintptr_t>(data_end_addr),
                               PAGE_SIZE);
             p += PAGE_SIZE)
        {
            uintptr_t phys = p - BootInfo::GetKernelVirtualAddress()
                           + BootInfo::GetKernelPhysicalAddress();
            kernelPageMap.MapPage(p, phys, PTE_PRESENT | PTE_WRITABLE);
        }

        uint64_t  entryCount = 0;
        MemoryMap memoryMap  = BootInfo::GetMemoryMap(entryCount);
        for (size_t i = 0; i < entryCount; i++)
        {
            MemoryMapEntry* entry = memoryMap[i];

            uintptr_t       base  = Math::AlignDown(entry->base, PAGE_SIZE);
            uintptr_t       top
                = Math::AlignUp(entry->base + entry->length, PAGE_SIZE);
            if (top <= 0x100000000) continue;

            for (uintptr_t j = base; j < top; j += PAGE_SIZE)
            {
                if (j < 0x100000000) continue;

                kernelPageMap.MapPage(ToHigherHalfAddress<uintptr_t>(j), j,
                                      PTE_PRESENT | PTE_WRITABLE);
            }
        }

        SwitchPageMap(kernelPageMap);
        LogInfo("Virtual Memory Manager Initialized");
    }

    void SwitchPageMap(PageMap& pageMap)
    {
        __asm__ volatile(
            "mov %0, %%cr3"
            :
            : "r"(FromHigherHalfAddress<uintptr_t>(pageMap.GetTopLevel()))
            : "memory");
    }

    PageMap& GetKernelPageMap() { return kernelPageMap; }

} // namespace VirtualMemoryManager