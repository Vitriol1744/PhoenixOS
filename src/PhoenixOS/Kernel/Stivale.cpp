#include "Stivale.hpp"

extern "C"
{
    static uint8_t stack[4096];

    static stivale2_header_tag_terminal terminal_hdr_tag = 
    {
        .tag = 
    	{
            .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
            .next = 0
        },
        .flags = 0
    };
    
    static stivale2_header_tag_framebuffer framebuffer_hdr_tag = 
    {
        .tag = 
        {
            .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
            .next = (uint64_t)&terminal_hdr_tag
        },
        .framebuffer_width  = 0,
        .framebuffer_height = 0,
        .framebuffer_bpp    = 0
    };
    
    __attribute__((section(".stivale2hdr"), used))
    static stivale2_header stivale_hdr = 
    {
        .entry_point = 0,
        .stack = (uintptr_t)stack + sizeof(stack),
        .flags = (1 << 1),
        .tags = (uintptr_t)&framebuffer_hdr_tag
    };
}

void* stivale2_get_tag(stivale2_struct* bootloader_data, uint64_t id) 
{
    stivale2_tag* current_tag = reinterpret_cast<stivale2_tag*>(bootloader_data->tags);
    while (true) 
	{
        if (!current_tag) return nullptr;
        if (current_tag->identifier == id) return current_tag;
 
        current_tag = reinterpret_cast<stivale2_tag*>(current_tag->next);
    }
}