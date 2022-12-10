#include "VirtualMemoryManager.hpp"

#include "BootInfo.hpp"
#include "Common.hpp"
#include "Memory/PhysicalMemoryManager.hpp"

#include "Utility/KLibC.hpp"

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
        return (uint64_t*)(PTE_GET_ADDR(level[nextLevelIndex])
                           + HIGHER_HALF_OFFSET);

    auto nextLevel   = reinterpret_cast<PhysAddr*>(PhysicalMemoryManager::CallocatePages(1));

    level[nextLevelIndex] = (uint64_t)nextLevel | (uint64_t)PTE_PRESENT
                          | (uint64_t)PTE_WRITABLE | PTE_USER_ACCESSIBLE;
    return (PhysAddr*)((uint8_t*)nextLevel + HIGHER_HALF_OFFSET);
}

#define PAGE_SIZE                   PhysicalMemoryManager::GetPageSize()
#define AlignUp(value, alignment)   (value + (alignment - value % alignment))
#define AlignDown(value, alignment) (value - (value % alignment))

static PageMap kernelPageMap;

#define PAGE_SIZE PhysicalMemoryManager::GetPageSize()

void PageMap::MapPage(VirtAddr virtAddr, PhysAddr physAddr, uint64_t flags)
{
    size_t    pml5Entry = (virtAddr >> 48) & 0x1ff;
    size_t    pml4Entry = (virtAddr & (0x1ffull << 39)) >> 39;
    size_t    pml3Entry = (virtAddr & (0x1ffull << 30)) >> 30;
    size_t    pml2Entry = (virtAddr & (0x1ffull << 21)) >> 21;
    size_t    pml1Entry = (virtAddr & (0x1ffull << 12)) >> 12;

    auto pml4      = reinterpret_cast<PhysAddr*>(GetTopLevel());
    PhysAddr* pml3      = GetNextLevel(pml4, pml4Entry);
    PhysAddr* pml2      = GetNextLevel(pml3, pml3Entry);
    PhysAddr* pml1      = GetNextLevel(pml2, pml2Entry);

    pml1[pml1Entry]     = physAddr | flags | PTE_USER_ACCESSIBLE;
    pml1[pml1Entry] &= ~PTE_EXECUTE_DISABLE;
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
            kernelPageMap.MapPage(addr + HIGHER_HALF_OFFSET, addr,
                                  PTE_PRESENT | PTE_WRITABLE);
        }

        for (auto p = AlignDown(reinterpret_cast<uintptr_t>(text_start_addr), PAGE_SIZE);
             p < AlignUp(reinterpret_cast<uintptr_t>(text_end_addr), PAGE_SIZE);
             p += PAGE_SIZE)
        {
            uintptr_t phys = p - BootInfo::GetKernelVirtualAddress()
                           + BootInfo::GetKernelPhysicalAddress();

            kernelPageMap.MapPage(p, phys,
                                  PTE_PRESENT | PTE_USER_ACCESSIBLE);
        }

        for (auto p= AlignDown(reinterpret_cast<uintptr_t>(rodata_start_addr), PAGE_SIZE);
             p < AlignUp(reinterpret_cast<uintptr_t>(rodata_end_addr), PAGE_SIZE);
             p += PAGE_SIZE)
        {
            uintptr_t phys = p - BootInfo::GetKernelVirtualAddress()
                           + BootInfo::GetKernelPhysicalAddress();
            kernelPageMap.MapPage(p, phys, PTE_PRESENT);
        }

        for (auto p = AlignDown(reinterpret_cast<uintptr_t>(data_start_addr), PAGE_SIZE);
             p < AlignUp(reinterpret_cast<uintptr_t>(data_end_addr), PAGE_SIZE);
             p += PAGE_SIZE)
        {
            uintptr_t phys = p - BootInfo::GetKernelVirtualAddress()
                           + BootInfo::GetKernelPhysicalAddress();
            kernelPageMap.MapPage(p, phys, PTE_PRESENT | PTE_WRITABLE);
        }

        uint64_t  entryCount = 0;
        MemoryMap memoryMap     = BootInfo::GetMemoryMap(entryCount);
        for (size_t i = 0; i < entryCount; i++)
        {
            MemoryMapEntry* entry = memoryMap[i];

            uintptr_t       base  = AlignDown(entry->base, PAGE_SIZE);
            uintptr_t top = AlignUp(entry->base + entry->length, PAGE_SIZE);
             if (top <= 0x100000000) continue;

            for (uintptr_t j = base; j < top; j += PAGE_SIZE)
            {
                 if (j < 0x100000000) continue;

                kernelPageMap.MapPage(j + HIGHER_HALF_OFFSET, j,
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
            : "r"(pageMap.GetTopLevel() - HIGHER_HALF_OFFSET)
            : "memory");
    }

    PageMap& GetKernelPageMap() { return kernelPageMap; }

} // namespace VirtualMemoryManager