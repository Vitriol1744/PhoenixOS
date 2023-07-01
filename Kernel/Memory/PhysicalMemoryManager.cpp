/*
 * Created by vitriol1744 on 28.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "PhysicalMemoryManager.hpp"
#undef PAGE_SIZE

#include "BootInfo.hpp"

#include "Memory/KernelHeap.hpp"
#include "Utility/Bitmap.hpp"
#include "Utility/Math.hpp"

#include <mutex>

namespace PhysicalMemoryManager
{
    inline static constexpr const uint32_t PAGE_SIZE = 0x1000;
    namespace
    {
        Bitmap     bitmap;
        uintptr_t  memoryTop   = 0;
        size_t     totalMemory = 0;
        size_t     usedMemory  = 0;
        std::mutex lock        = {};

        void* FindFreeRegion(uint64_t& start, uint64_t count, uint64_t limit)
        {
            size_t contiguousPages = 0;
            while (start < limit)
            {
                if (!bitmap.GetIndex(start++))
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
    } // namespace

    bool Initialize()
    {
        uint64_t  entryCount = 0;
        MemoryMap memoryMap  = BootInfo::GetMemoryMap(entryCount);
        if (entryCount == 0) return false;

        for (size_t i = 0; i < entryCount; i++)
        {
            MemoryMapEntry* currentEntry = memoryMap[i];
            switch (currentEntry->type)
            {
                case MEMORY_MAP_USABLE:
                    totalMemory += currentEntry->length;
                    if (currentEntry->base + currentEntry->length > memoryTop)
                        memoryTop = currentEntry->base + currentEntry->length;
                    break;
                case MEMORY_MAP_ACPI_RECLAIMABLE:
                case MEMORY_MAP_BOOTLOADER_RECLAIMABLE:
                case MEMORY_MAP_KERNEL_AND_MODULES:
                    totalMemory += currentEntry->length;
                    usedMemory += currentEntry->length;
                    break;
                default: break;
            }
        }

        if (memoryTop == 0) return false;

        uint32_t bitmapSize
            = Math::AlignUp(memoryTop / PAGE_SIZE / 8, PAGE_SIZE);
        for (size_t i = 0; i < entryCount; i++)
        {
            // Find the biggest entry
            MemoryMapEntry* currentEntry = memoryMap[i];
            if (currentEntry->type == MEMORY_MAP_USABLE
                && currentEntry->length > bitmapSize)
            {
                bitmap.data = reinterpret_cast<uint8_t*>(
                    currentEntry->base + BootInfo::GetHHDMOffset());
                bitmap.size = bitmapSize;
                bitmap.SetAll(0xff);
                currentEntry->base += bitmapSize;
                currentEntry->length -= bitmapSize;
                break;
            }
        }

        // Mark used pages
        for (size_t i = 0; i < entryCount; i++)
        {
            MemoryMapEntry* currentEntry = memoryMap[i];
            if (currentEntry->type != MEMORY_MAP_USABLE) continue;

            for (uintptr_t page = 0; page < currentEntry->length;
                 page += PAGE_SIZE)
                bitmap.SetIndex((currentEntry->base + page) / PAGE_SIZE, false);
        }

        memoryTop = Math::AlignUp(memoryTop, 0x40000000);

        KernelHeap::Initialize();

        LogInfo(
            "Memory Map entries count: {}, Total Memory: {}, Free Memory: {}",
            entryCount, totalMemory, GetFreeMemory());
        return true;
    }
    void* AllocatePages(size_t count)
    {
        std::unique_lock guard(lock);
        if (count == 0) return nullptr;
        static uint64_t lastIndex = 0;

        size_t          i         = lastIndex;
        void* ret = FindFreeRegion(lastIndex, count, memoryTop / PAGE_SIZE);
        if (!ret)
        {
            lastIndex = 0;
            ret       = FindFreeRegion(lastIndex, count, i);
            if (!ret) panic("Out of memory!");
        }

        usedMemory += count * PAGE_SIZE;
        return ret;
    }
    void* CallocatePages(size_t count)
    {
        void* ret = AllocatePages(count);
        if (!ret) return nullptr;

        uintptr_t higherHalfAddress
            = reinterpret_cast<uintptr_t>(ret) + BootInfo::GetHHDMOffset();
        memset(reinterpret_cast<void*>(higherHalfAddress), 0,
               PAGE_SIZE * count);

        return ret;
    }
    void FreePages(void* ptr, size_t count)
    {
        std::unique_lock guard(lock);
        if (count == 0 || !ptr) return;
        uintptr_t page = reinterpret_cast<uintptr_t>(ptr) / PAGE_SIZE;

        for (uintptr_t i = page; i < page + count; i++)
            bitmap.SetIndex(i, false);
        usedMemory -= count * PAGE_SIZE;
    }

    uint64_t GetPageSize() { return PAGE_SIZE; }
    uint64_t GetTotalMemory() { return totalMemory; }
    uint64_t GetFreeMemory() { return totalMemory - usedMemory; }
    uint64_t GetUsedMemory() { return usedMemory; }
} // namespace PhysicalMemoryManager