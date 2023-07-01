/*
 * Created by vitriol1744 on 28.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include <cstddef>
#include <cstdint>

#define PAGE_SIZE PhysicalMemoryManager::GetPageSize()

namespace PhysicalMemoryManager
{
    bool Initialize();
    void* AllocatePages(size_t count = 1);
    void* CallocatePages(size_t count = 1);
    void FreePages(void* ptr, size_t count);

    template <typename T>
    T AllocatePages(size_t count = 1) { return reinterpret_cast<T>(AllocatePages(count)); }
    template <typename T>
    T CallocatePages(size_t count = 1) { return reinterpret_cast<T>(CallocatePages(count)); }
    template <typename T>
    void FreePages(T ptr, size_t count) { FreePages(reinterpret_cast<void*>(ptr), count); }

    uint64_t GetPageSize();
    uint64_t GetTotalMemory();
    uint64_t GetFreeMemory();
    uint64_t GetUsedMemory();
}