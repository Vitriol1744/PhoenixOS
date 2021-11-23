#ifndef PHYSICAL_MEMORY_ALLOCATOR_H
#define PHYSICAL_MEMORY_ALLOCATOR_H

#include "common.h"
#include "kernel/stivale.h"

extern void physicalMemoryAllocator_Initialize(stivale2_mmap_entry_t* memory_map,
                                              size_t               entries);

extern void* physicalMemoryAllocator_AllocatePages(size_t num);

#endif // PHYSICAL_MEMORY_ALLOCATOR_H