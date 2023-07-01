/*
 * Created by vitriol1744 on 28.11.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include <stddef.h>
#include <stdint.h>

namespace KernelHeap
{
    void  Initialize();
    void* Allocate(size_t bytes);
    void* Callocate(size_t bytes);
    void* Reallocate(void* ptr, size_t size);
    void  Free(void* memory);

    template <typename T>
    inline T Allocate(size_t bytes)
    {
        return reinterpret_cast<T>(Allocate(bytes));
    }
    template <typename T>
    inline T Callocate(size_t bytes)
    {
        return reinterpret_cast<T>(Callocate(bytes));
    }
    template <typename T>
    T Reallocate(T ptr, size_t size)
    {
        return reinterpret_cast<T>(
            Reallocate(reinterpret_cast<void*>(ptr), size));
    }

    template <typename T>
    void Free(T memory)
    {
        return Free(reinterpret_cast<void*>(memory));
    }
} // namespace KernelHeap