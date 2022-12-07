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