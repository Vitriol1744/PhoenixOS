#include "VirtualMemoryManager.hpp"

#include "BootInfo.hpp"
#include "Common.hpp"
#include "Memory/PhysicalMemoryManager.hpp"

#include "Utility/KLibC.hpp"

#include <stdbool.h>

extern "C" symbol text_start_addr;
extern "C" symbol text_end_addr;
extern "C" symbol rodata_start_addr;
extern "C" symbol rodata_end_addr;
extern "C" symbol data_start_addr;
extern "C" symbol data_end_addr;

#define PTE_ADDR_MASK       0x000ffffffffff000
#define PTE_GET_ADDR(value) ((value)&PTE_ADDR_MASK)
#define VMM_HIGHER_HALF     BootInfo::GetHHDMOffset()

static PhysAddr* GetNextLevel(PhysAddr* level, uint64_t nextLevelIndex)
{
    if ((level[nextLevelIndex] & PTE_PRESENT) != 0)
        return (uint64_t*)(PTE_GET_ADDR(level[nextLevelIndex])
                           + VMM_HIGHER_HALF);

    PhysAddr* nextLevel   = (PhysAddr*)PhysicalMemoryManager::CallocatePages(1);
    *nextLevel            = 8;

    level[nextLevelIndex] = (uint64_t)nextLevel | PTE_PRESENT | PTE_WRITABLE
                          | PTE_EXECUTE_DISABLE;
    return (PhysAddr*)((uint8_t*)nextLevel + VMM_HIGHER_HALF);
}

#define PAGE_SIZE                   PhysicalMemoryManager::GetPageSize()
#define AlignUp(value, alignment)   (value + (alignment - value % alignment))
#define AlignDown(value, alignment) (value - (value % alignment))

static PageMap kernelPageMap;

#define PAGE_SIZE PhysicalMemoryManager::GetPageSize()

void PageMap::MapPage(PhysAddr physAddr, VirtAddr virtAddr, uint64_t flags)
{
    size_t    pml5Entry = (virtAddr >> 48) & 0x1ff;
    size_t    pml4Entry = (virtAddr >> 39) & 0x1ff;
    size_t    pml3Entry = (virtAddr >> 30) & 0x1ff;
    size_t    pml2Entry = (virtAddr >> 21) & 0x1ff;
    size_t    pml1Entry = (virtAddr >> 12) & 0x1ff;

    PhysAddr* pml4      = reinterpret_cast<PhysAddr*>(topLevel);
    PhysAddr* pml3      = GetNextLevel(pml4, pml4Entry);
    PhysAddr* pml2      = GetNextLevel(pml3, pml3Entry);
    PhysAddr* pml1      = GetNextLevel(pml2, pml2Entry);
    pml1[pml1Entry]     = physAddr | flags;
}

namespace VirtualMemoryManager
{
    void Initialize()
    {
        kernelPageMap.Create();

        for (size_t i = 256; i < 512; i++)
            Assert(GetNextLevel((PhysAddr*)kernelPageMap.GetTopLevel(), i)
                   != 0);

        for (uintptr_t addr = 0x1000; addr < 0x100000000; addr += PAGE_SIZE)
            kernelPageMap.MapPage(addr, addr + VMM_HIGHER_HALF,
                                  PTE_PRESENT | PTE_WRITABLE
                                      | PTE_EXECUTE_DISABLE);

        uint64_t  entryCount = 0;
        MemoryMap memmap     = BootInfo::GetMemoryMap(entryCount);
        for (size_t i = 0; i < entryCount; i++)
        {
            MemoryMapEntry* entry = memmap[i];

            uintptr_t       base  = AlignDown(entry->base, PAGE_SIZE);
            uintptr_t top = AlignUp(entry->base + entry->length, PAGE_SIZE);
            if (top <= 0x100000000) continue;

            for (uintptr_t j = base; j < top; j += PAGE_SIZE)
            {
                if (j < 0x100000000) continue;

                kernelPageMap.MapPage(j, j + VMM_HIGHER_HALF,
                                      PTE_PRESENT | PTE_WRITABLE
                                          | PTE_EXECUTE_DISABLE);
            }
        }
        uint64_t kernelPhys = BootInfo::GetKernelPhysicalAddress();
        uint64_t kernelVirt = BootInfo::GetKernelVirtualAddress();

        for (PhysAddr p = AlignDown((PhysAddr)text_start_addr, PAGE_SIZE);
             p < AlignUp((PhysAddr)text_end_addr, PAGE_SIZE); p += PAGE_SIZE)
            kernelPageMap.MapPage(p - kernelVirt + kernelPhys, p, PTE_PRESENT);
        for (PhysAddr p = AlignDown((PhysAddr)rodata_start_addr, PAGE_SIZE);
             p < AlignUp((PhysAddr)rodata_end_addr, PAGE_SIZE); p += PAGE_SIZE)
            kernelPageMap.MapPage(p - kernelVirt + kernelPhys, p, PTE_PRESENT);
        for (PhysAddr p = AlignDown((PhysAddr)data_start_addr, PAGE_SIZE);
             p < AlignUp((PhysAddr)data_end_addr, PAGE_SIZE); p += PAGE_SIZE)
            kernelPageMap.MapPage(p - kernelVirt + kernelPhys, p,
                                  PTE_PRESENT | PTE_WRITABLE);

        SwitchPageMap(kernelPageMap);
        LogInfo("Virtual Memory Manager Initialized");
    }

    void SwitchPageMap(PageMap& pageMap)
    {
        __asm__ __volatile__("mov %%cr3, %0"
                             :
                             : "r"(pageMap.GetTopLevel())
                             : "memory");
    }

    PageMap& GetKernelPageMap() { return kernelPageMap; }

} // namespace VirtualMemoryManager