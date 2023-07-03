/*
 * Created by vitriol1744 on 23.12.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "BootInfo.hpp"
#include "Common.hpp"

#include "Memory/PhysicalMemoryManager.hpp"
#include "Memory/VirtualMemoryManager.hpp"
#include "Utility/Math.hpp"

#include <mutex>

class SlabAllocatorBase
{
  public:
    virtual void*  Allocate()          = 0;
    virtual void   Free(void* ptr)     = 0;
    virtual size_t GetAllocationSize() = 0;
};

struct SlabHeader
{
    SlabAllocatorBase* slab;
};

template <uint64_t Bytes>
class SlabAllocator : public SlabAllocatorBase
{
  public:
    SlabAllocator() = default;
    void Initialize()
    {
        firstFree = ToHigherHalfAddress<uintptr_t>(reinterpret_cast<uintptr_t>(
            PhysicalMemoryManager::CallocatePages(1)));
        auto available
            = 0x1000 - Math::AlignUp(sizeof(SlabHeader), allocationSize);
        auto slabPointer  = reinterpret_cast<SlabHeader*>(firstFree);
        slabPointer->slab = this;
        firstFree += Math::AlignUp(sizeof(SlabHeader), allocationSize);

        auto array = reinterpret_cast<uint64_t*>(firstFree);
        auto max   = available / allocationSize - 1;
        auto fact  = allocationSize / 8;
        for (size_t i = 0; i < max; i++)
            array[i * fact]
                = reinterpret_cast<uint64_t>(&array[(i + 1) * fact]);
        array[max * fact] = 0;
    }

    void* Allocate() override
    {
        std::unique_lock guard(lock);
        if (!firstFree) Initialize();

        auto oldFree = reinterpret_cast<uint64_t*>(firstFree);
        firstFree    = oldFree[0];
        memset(oldFree, 0, allocationSize);

        return oldFree;
    }
    void Free(void* memory) override
    {
        std::unique_lock guard(lock);
        if (!memory) return;

        auto newHead = static_cast<uint64_t*>(memory);
        newHead[0]   = firstFree;
        firstFree    = reinterpret_cast<uintptr_t>(newHead);
    }
    virtual size_t GetAllocationSize() override { return allocationSize; }

    template <typename T>
    T Allocate()
    {
        return reinterpret_cast<T>(Allocate());
    }
    template <typename T>
    void Free(T memory)
    {
        return Free(reinterpret_cast<void*>(memory));
    }

  private:
    uintptr_t  firstFree      = 0;
    size_t     allocationSize = Bytes;
    std::mutex lock;
};
