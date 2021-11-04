#ifndef PHYSICAL_MEMORY_ALLOCATOR_H
#define PHYSICAL_MEMORY_ALLOCATOR_H

#include "common.h"
#include "stivale2.h"

extern void physicalMemoryAllocatorInitialize(stivale2_mmap_entry* memory_map,
                                              size_t               entries);

#endif // PHYSICAL_MEMORY_ALLOCATOR_H