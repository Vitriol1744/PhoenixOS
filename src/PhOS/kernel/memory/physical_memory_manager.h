#ifndef PHYSICAL_MEMORY_MANAGER_H
#define PHYSICAL_MEMORY_MANAGER_H

#include "common.h"
#include "kernel/stivale.h"

extern void  pmm_Initialize(stivale2_mmap_entry_t* memory_map,
                            size_t                 entries_count);

extern void* pmm_AllocatePages(size_t num);

#endif // PHYSICAL_MEMORY_MANAGER_H