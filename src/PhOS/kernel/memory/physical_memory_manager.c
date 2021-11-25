#include "physical_memory_allocator.h"

#include "bitmap.h"

bitmap_t bitmap;
uint32_t bitmaps_count = 0;

PH_CONSTEXPR(PAGE_SIZE, 0x1000); // 4K
uint64_t highest_page = 0;

void pmm_Initialize(stivale2_mmap_entry_t* memory_map, size_t entries)
{
    size_t total_memory = 0;
    for (size_t i = 0; i < entries; i++) 
    {
        PH_LOG_INFO("Entry[%i] - Base: 0x%x, Size: 0x%x, Type: 0x%x", i, memory_map[i].base,
			memory_map[i].length, memory_map[i].type);
        total_memory += memory_map[i].length;
    }
    PH_LOG_INFO("Total Memory: %d", total_memory);
    PH_ASM("hlt");
    for (size_t i = 0; i < entries; ++i)
	{
		if (memory_map[i].type != STIVALE2_MMAP_USABLE) continue;

		uintptr_t top_page = memory_map[i].base + memory_map[i].length;
		if (top_page > highest_page) highest_page = top_page;
	}

    size_t bitmap_size = (highest_page + (PAGE_SIZE - 1)) / PAGE_SIZE / 8;
	for (size_t i = 0; i < entries; ++i)
	{
		if (memory_map[i].type != STIVALE2_MMAP_USABLE) continue;

		if (memory_map[i].length >= bitmap_size)
		{
			bitmap.indices = ((uint8_t*)(memory_map[i].base + 0xFFFF800000000000));
			bitmap.size = bitmap_size;

			for (size_t i = 0; i < bitmap.size; ++i) bitmap.indices[i] = 0xFF;

			memory_map[i].length -= bitmap_size;
			memory_map[i].base += bitmap_size;
			break;
		}
	}

	for (size_t i = 0; i < entries; ++i)
	{
		if (memory_map[i].type != STIVALE2_MMAP_USABLE) continue;

		for (uintptr_t j = 0; j < memory_map[i].length; j += PAGE_SIZE)
		{
            size_t position = (memory_map[i].base + j) / PAGE_SIZE;
            if (position > bitmap.size) continue;
			
            uint64_t byte_index;
		    uint8_t bit_value;
            byte_index = position / 8;
		    bit_value = (uint8_t)(1 << (position % 8));

		    bitmap.indices[byte_index] &= ~bit_value;
		}
	}
}

void* pmm_AllocatePages(size_t num)
{
    static size_t last_used_index = 0;
    if (last_used_index > bitmap.size) last_used_index = 0;

    size_t p = 0;
    while (last_used_index < highest_page / PAGE_SIZE)
    {
        if (bitmap_GetIndex(&bitmap, last_used_index++))
        {
            p = 0;
            continue;
        }

        if (p++ != num) continue;
        size_t page = last_used_index;
        for (size_t i = page; i < last_used_index; i++) bitmap_SetIndex(&bitmap, i, true);

        //PH_LOG_INFO("Allocated %d bytes of memory!", num * PAGE_SIZE);
        PH_LOG_INFO("Index: %d, MaxIndex: %d, c: %d", last_used_index, bitmap.size, highest_page / PAGE_SIZE);
        return (void*)(page * PAGE_SIZE);
    }
    
    PH_LOG_INFO("Out of Memory!");
    return PH_NULL_HANDLE;
}
void pmm_FreePages(void* address, size_t num)
{
    size_t page_index = (uint64_t)address / PAGE_SIZE;
    num += page_index;
    for (; page_index < num; page_index++) bitmap_SetIndex(&bitmap, page_index, false);
}