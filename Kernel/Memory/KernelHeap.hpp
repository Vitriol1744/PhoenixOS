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
    void* Allocate(size_t bytes, uintptr_t alignment);
    void* Callocate(size_t bytes);
    void* Callocate(size_t bytes, uintptr_t alignment);
    void  Free(void* memory);
} // namespace KernelHeap