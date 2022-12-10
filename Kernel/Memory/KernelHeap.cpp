/*
 * Created by vitriol1744 on 28.11.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "KernelHeap.hpp"

#include "BootInfo.hpp"
#include "Common.hpp"

#include "Memory/PhysicalMemoryManager.hpp"
#include "Scheduler/Spinlock.hpp"
#include "Utility/KLibC.hpp"

struct MemorySegment
{
    MemorySegment* previousSegment = nullptr;
    MemorySegment* nextSegment     = nullptr;
    uint32_t       length          = 0;
    bool           free            = true;
};

void* operator new(size_t size) { return KernelHeap::Callocate(size, 0); }
void* operator new[](size_t size) { return KernelHeap::Callocate(size, 0); }
void  operator delete(void* ptr) noexcept { KernelHeap::Free(ptr); }
void  operator delete[](void* ptr) noexcept { KernelHeap::Free(ptr); }

static MemorySegment* firstSegment = nullptr;
static Spinlock       lock         = {};
static void           DoPrintFreeSpace()
{
    uint32_t       freeSpace      = 0;
    uint32_t       segmentCount   = 0;
    MemorySegment* currentSegment = firstSegment;
    while (currentSegment)
    {
        LogInfo("Segment[%d]: ptr %#014p, size: %lld, free %lld", segmentCount,
                (currentSegment + 1), currentSegment->length,
                currentSegment->free);
        segmentCount++;
        if (currentSegment->free) freeSpace += currentSegment->length;
        currentSegment = currentSegment->nextSegment;
    }
    LogInfo("Free Space: %d, SegmentCount: %d", freeSpace, segmentCount);
}
#if 0
    #define PrintFreeSpace() DoPrintFreeSpace()
#else
    #define PrintFreeSpace()
#endif

// This kernel heap is a bit complicated because of implementing
// AlignedAlloc, but we will eventually rewrite all of it, it is just a
// quick temporary solution just to be able to work on stuff requiring
// dynamic allocation.
namespace KernelHeap
{

    void Initialize()
    {
        static constexpr const uint32_t maxPages = 16;
        const uint32_t                  totalHeapMemory
            = maxPages * PhysicalMemoryManager::GetPageSize();

        firstSegment = reinterpret_cast<MemorySegment*>(
            (uint8_t*)PhysicalMemoryManager::AllocatePages(maxPages)
            + BootInfo::GetHHDMOffset());
        firstSegment->previousSegment = nullptr;
        firstSegment->nextSegment     = nullptr;
        firstSegment->length          = totalHeapMemory - sizeof(MemorySegment);
        firstSegment->free            = true;
    }

    void* Allocate(size_t bytes, uintptr_t alignment = 0)
    {
        lock.Lock();
        if (!firstSegment) panic("Kernel Heap is not initialized!");
        MemorySegment* currentSegment = firstSegment;

        uint32_t       toAlign        = 0;
        while (currentSegment)
        {
            toAlign = alignment
                        ? alignment
                              - reinterpret_cast<uintptr_t>(currentSegment + 1)
                                    % alignment
                        : 0;
            if (currentSegment->free
                && currentSegment->length >= bytes + toAlign)
                break;
            currentSegment = currentSegment->nextSegment;
        }
        if (!currentSegment) panic("Out of Memory!");

        uint32_t nextSegmentLength
            = currentSegment->length - bytes - sizeof(MemorySegment) - toAlign;
        currentSegment->free       = false;
        currentSegment->length     = bytes + toAlign;

        auto nextSegment = reinterpret_cast<MemorySegment*>(
            reinterpret_cast<uint8_t*>(currentSegment + 1)
            + currentSegment->length);
        nextSegment->nextSegment = currentSegment->nextSegment;
        if (currentSegment->nextSegment)
            currentSegment->nextSegment->previousSegment = nextSegment;
        currentSegment->nextSegment  = nextSegment;

        nextSegment->previousSegment = currentSegment;
        nextSegment->length          = nextSegmentLength;
        nextSegment->free            = true;

        PrintFreeSpace();
        lock.Unlock();
        return reinterpret_cast<uint8_t*>(currentSegment + 1) + toAlign;
    }
    void* Callocate(size_t bytes, uintptr_t alignment)
    {
        void* ret = Allocate(bytes, alignment);
        memset(ret, 0, bytes);

        return ret;
    }
    void Free(void* memory)
    {
        if (!memory) return;
        lock.Lock();

#define InRange(val, start, end) (val >= start && val < end)
        MemorySegment* currentSegment = firstSegment;
        while (currentSegment)
        {
            if (!currentSegment->free)
            {
                auto segmentStart
                    = reinterpret_cast<uint8_t*>(currentSegment + 1);
                uint8_t* segmentEnd = segmentStart + currentSegment->length;
                if (InRange(memory, segmentStart, segmentEnd))
                {
                    currentSegment->free = true;
                    break;
                }
            }
            currentSegment = currentSegment->nextSegment;
        }

        while (currentSegment)
        {
            if (currentSegment->previousSegment != nullptr
                && currentSegment->previousSegment->free)
            {
                currentSegment = currentSegment->previousSegment;
                continue;
            }
            break;
        }
        MemorySegment* toConcatSegment = currentSegment->nextSegment;
        if (!currentSegment->nextSegment->free) goto cleanup;
        while (toConcatSegment && toConcatSegment->free)
        {
            currentSegment->nextSegment = toConcatSegment->nextSegment;
            currentSegment->length += toConcatSegment->length;
            toConcatSegment = currentSegment->nextSegment;
        }
    cleanup:
        PrintFreeSpace();
        lock.Unlock();
    }
} // namespace KernelHeap