/*
 * Created by vitriol1744 on 28.11.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "KernelHeap.hpp"

#include "Common.hpp"

#include "Memory/PhysicalMemoryManager.hpp"
#include "Memory/SlabAllocator.hpp"

namespace
{
    SlabAllocator<8>    allocator8    = {};
    SlabAllocator<16>   allocator16   = {};
    SlabAllocator<32>   allocator32   = {};
    SlabAllocator<64>   allocator64   = {};
    SlabAllocator<128>  allocator128  = {};
    SlabAllocator<256>  allocator256  = {};
    SlabAllocator<512>  allocator512  = {};
    SlabAllocator<1024> allocator1024 = {};

    struct BigAllocMeta
    {
        size_t pages;
        size_t size;
    };
    void* InternalAllocate(size_t size)
    {
        if (size <= 8) return allocator8.Allocate<void*>();
        else if (size <= 16) return allocator16.Allocate<void*>();
        else if (size <= 32) return allocator32.Allocate<void*>();
        else if (size <= 64) return allocator64.Allocate<void*>();
        else if (size <= 128) return allocator128.Allocate<void*>();
        else if (size <= 256) return allocator256.Allocate<void*>();
        else if (size <= 512) return allocator512.Allocate<void*>();
        else if (size <= 1024) return allocator1024.Allocate<void*>();

        size_t pages = Math::AlignUp(size, 4096) / 4096;
        auto ptr = PhysicalMemoryManager::CallocatePages<uintptr_t>(pages + 1);
        if (ptr == 0) return nullptr;

        auto meta
            = reinterpret_cast<BigAllocMeta*>(ToHigherHalfAddress<void*>(ptr));
        meta->pages = pages;
        meta->size  = size;

        return reinterpret_cast<void*>(ToHigherHalfAddress<uintptr_t>(ptr)
                                       + PAGE_SIZE);
    }
    void InternalFree(void* ptr)
    {
        if ((reinterpret_cast<uintptr_t>(ptr) & 0xfff) == 0)
        {
            auto meta = reinterpret_cast<BigAllocMeta*>(
                reinterpret_cast<uintptr_t>(ptr) - PAGE_SIZE);
            PhysicalMemoryManager::FreePages(
                FromHigherHalfAddress<void*>((uintptr_t)meta), meta->pages + 1);
            return;
        }
        reinterpret_cast<SlabHeader*>(reinterpret_cast<uintptr_t>(ptr) & ~0xfff)
            ->slab->Free(ptr);
    }
} // namespace

namespace KernelHeap
{
    void Initialize()
    {
        allocator8.Initialize();
        allocator16.Initialize();
        allocator32.Initialize();
        allocator64.Initialize();
        allocator128.Initialize();
        allocator256.Initialize();
        allocator512.Initialize();
        allocator1024.Initialize();
    }

    void* Allocate(size_t bytes) { return InternalAllocate(bytes); }
    void* Callocate(size_t bytes)
    {
        void* ret = InternalAllocate(bytes);
        if (!ret) return nullptr;

        memset(ret, 0, bytes);
        return ret;
    }
    // TODO(V1tri0l1744): Reallocate
    void* Reallocate(void* ptr, size_t size)
    {
        if (!ptr) return Allocate(size);
        if ((reinterpret_cast<uintptr_t>(ptr) & 0xfff) == 0)
        {
            BigAllocMeta* metadata = reinterpret_cast<BigAllocMeta*>(
                reinterpret_cast<uintptr_t>(ptr) - 0x1000);
            size_t oldSize = metadata->size;

            if (Math::DivRoundUp(oldSize, 0x1000)
                == Math::DivRoundUp(size, 0x1000))
            {
                metadata->size = size;
                return ptr;
            }

            if (size == 0)
            {
                Free(ptr);
                return nullptr;
            }

            if (size < oldSize) oldSize = size;

            void* ret = Allocate(size);
            if (!ret) return ptr;

            memcpy(ret, ptr, oldSize);
            Free(ptr);
            return ret;
        }

        SlabAllocatorBase* slab = reinterpret_cast<SlabHeader*>(
                                      reinterpret_cast<uintptr_t>(ptr) & ~0xFFF)
                                      ->slab;
        size_t oldSize = slab->GetAllocationSize();

        if (size == 0)
        {
            Free(ptr);
            return nullptr;
        }
        if (size < oldSize) oldSize = size;

        void* ret = Allocate(size);
        if (!ret) return ptr;

        memcpy(ret, ptr, oldSize);
        Free(ptr);
        return ptr;
    }
    void Free(void* memory)
    {
        if (!memory) return;
        InternalFree(memory);
    }
} // namespace KernelHeap

void* operator new(size_t size) { return KernelHeap::Callocate(size); }
void* operator new(size_t size, std::align_val_t)
{
    return KernelHeap::Callocate(size);
}
void* operator new[](size_t size) { return KernelHeap::Callocate(size); }
void* operator new[](size_t size, std::align_val_t)
{
    return KernelHeap::Callocate(size);
}
void operator delete(void* ptr) noexcept { KernelHeap::Free(ptr); }
void operator delete(void* ptr, std::align_val_t) noexcept
{
    KernelHeap::Free(ptr);
}
void operator delete(void* ptr, size_t) noexcept { KernelHeap::Free(ptr); }
void operator delete[](void* ptr) noexcept { KernelHeap::Free(ptr); }
void operator delete[](void* ptr, std::align_val_t) noexcept
{
    KernelHeap::Free(ptr);
}
void operator delete[](void* ptr, size_t) noexcept { KernelHeap::Free(ptr); }
