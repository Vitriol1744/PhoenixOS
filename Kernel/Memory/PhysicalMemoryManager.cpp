#include "PhysicalMemoryManager.hpp"

#include "BootInfo.hpp"

#include "Memory/KernelHeap.hpp"
#include "Utility/Bitmap.hpp"

#include "Scheduler/Spinlock.hpp"

inline static constexpr const uint32_t PAGE_SIZE = 0x1000;

namespace PhysicalMemoryManager
{
    static Bitmap    bitmap;
    static uintptr_t memoryTop   = 0;
    static size_t    totalMemory = 0;
    static size_t    freeMemory  = 0;
    static Spinlock  lock        = {};

    bool             Initialize()
    {
        uint64_t  entryCount = 0;
        MemoryMap memoryMap  = BootInfo::GetMemoryMap(entryCount);
        for (uint32_t i = 0; i < entryCount; i++)
        {
            //LogTrace(
              //  "MemoryMapEntry[%02d]: Base: %#010p, Length: %#010lx, Type: "
                //"%ld\n",
                //i, memoryMap[i]->base, memoryMap[i]->length,
                //memoryMap[i]->type);

            switch (memoryMap[i]->type)
            {
                case MEMORY_MAP_USABLE:
                    if (memoryMap[i]->base + memoryMap[i]->length > memoryTop)
                        memoryTop = memoryMap[i]->base + memoryMap[i]->length;
                    freeMemory += memoryMap[i]->length;
                case MEMORY_MAP_ACPI_RECLAIMABLE:
                case MEMORY_MAP_BOOTLOADER_RECLAIMABLE:
                    totalMemory += memoryMap[i]->length;
                    break;
                default: break;
            }
        }
	LogInfo("Memory Map entries count: %lld, Total Memory: %lld, Free Memory: %lld", entryCount, totalMemory, freeMemory);

        uint32_t bitmapSize = memoryTop / PAGE_SIZE / 8;
        bitmapSize += 4096 - bitmapSize % 4096;
        for (uint32_t i = 0; i < entryCount; i++)
        {
            if (memoryMap[i]->type == MEMORY_MAP_USABLE
                && memoryMap[i]->length > bitmapSize)
            {
                bitmap.data
                    = (uint8_t*)memoryMap[i]->base + BootInfo::GetHHDMOffset();
                bitmap.size = bitmapSize;
                bitmap.SetAll(0xff);
                memoryMap[i]->base += bitmapSize;
                memoryMap[i]->length -= bitmapSize;
                break;
            }
        }
        for (uint64_t i = 0; i < entryCount; i++)
        {
            if (memoryMap[i]->type != MEMORY_MAP_USABLE) continue;

            for (uintptr_t page = 0; page < memoryMap[i]->length;
                 page += PAGE_SIZE)
                bitmap.SetIndex((memoryMap[i]->base + page) / PAGE_SIZE, false);
        }
        KernelHeap::Initialize();
        return true;
    }

    static void* FindFreeRegion(uint64_t& start, uint64_t count, uint64_t limit)
    {
        size_t contiguousPages = 0;
        while (start < limit)
        {
            if (bitmap.GetIndex(start++) == false)
            {
                if (++contiguousPages == count)
                {
                    size_t page = start - count;
                    for (size_t i = page; i < start; i++)
                        bitmap.SetIndex(i, true);
                    return reinterpret_cast<void*>(page * PAGE_SIZE);
                }
            }
            else contiguousPages = 0;
        }
        return nullptr;
    }
    void* AllocatePages(size_t count)
    {
        lock.Lock();
        static uint64_t lastIndex = 0;
        if (count == 0) return nullptr;

        size_t i   = lastIndex;
        void*  ret = FindFreeRegion(lastIndex, count, memoryTop / PAGE_SIZE);
        if (!ret)
        {
            lastIndex = 0;
            ret       = FindFreeRegion(lastIndex, count, i);
            if (!ret) panic("Out of memory!");
        }

        freeMemory -= count * PAGE_SIZE;
        lock.Unlock();
        return ret;
    }
    void* CallocatePages(size_t count)
    {
        void* ret = AllocatePages(count);
        memset((uint8_t*)ret + BootInfo::GetHHDMOffset(), 0, count * PAGE_SIZE);

        return ret;
    }
    void FreePages(void* ptr, size_t count)
    {
        if (count == 0) return;
        uintptr_t page = reinterpret_cast<uintptr_t>(ptr) / PAGE_SIZE;
        lock.Lock();
        for (uintptr_t i = page; i < page + count; i++)
            bitmap.SetIndex(i, false);
        freeMemory += count * PAGE_SIZE;
        lock.Unlock();
    }

    uint32_t GetPageSize() { return PAGE_SIZE; }
    uint64_t GetTotalMemory() { return totalMemory; }
    uint64_t GetFreeMemory() { return freeMemory; }
    uint64_t GetUsedMemory() { return totalMemory - freeMemory; }
} // namespace PhysicalMemoryManager
