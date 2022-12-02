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