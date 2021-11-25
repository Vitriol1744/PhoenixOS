#ifndef VIRTUAL_MEMORY_MANAGER
#define VIRTUAL_MEMORY_MANAGER

#include "common.h"
#include "kernel/stivale.h"

extern void vmm_Initialize(stivale2_mmap_entry_t* memory_map,
                           size_t                 entries_count);

#endif // VIRTUAL_MEMORY_MANAGER