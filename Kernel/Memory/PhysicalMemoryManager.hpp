/*
 * Created by vitriol1744 on 27.11.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Common.hpp"

namespace PhysicalMemoryManager
{
    bool     Initialize();
    void*    AllocatePages(size_t count);
    void*    CallocatePages(size_t count);
    void     FreePages(void* ptr, size_t count);

    uint32_t GetPageSize();
    uint64_t GetTotalMemory();
    uint64_t GetFreeMemory();
    uint64_t GetUsedMemory();
} // namespace PhysicalMemoryManager