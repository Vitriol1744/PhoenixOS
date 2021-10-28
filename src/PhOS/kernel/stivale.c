#include "stivale.h"

static uint8_t stack[4096];

static stivale2_header_tag_terminal_t terminal_hdr_tag = 
{
    .tag = 
    {
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        .next = 0
    },
    .flags = 0
};
    
static stivale2_header_tag_framebuffer_t framebuffer_hdr_tag = 
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
static stivale2_header_t stivale_hdr = 
{
    .entry_point = 0,
    .stack = (uintptr_t)stack + sizeof(stack),
    .flags = (1 << 1),
    .tags = (uintptr_t)&framebuffer_hdr_tag
};

void* stivale2GetTag(stivale2_struct_t* bootloader_data, uint64_t id) 
{
    stivale2_tag_t* current_tag = (stivale2_tag_t*)(bootloader_data->tags);
    while (true) 
    {
        if (!current_tag) return PH_NULL_HANDLE;
        if (current_tag->identifier == id) return current_tag;
 
        current_tag = (stivale2_tag_t*)(current_tag->next);
    }
}   