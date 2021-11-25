#include "terminal.h"

void (*terminal_write)(const char* string, size_t size);

void terminal_Initialize(stivale2_struct_t* bootloader_data)
{
    struct stivale2_struct_tag_terminal* term_str_tag
        = stivale2GetTag(bootloader_data, STIVALE2_STRUCT_TAG_TERMINAL_ID);

    if (!term_str_tag)
    {
        for (;;) PH_ASM("hlt");
    }

    terminal_write = (void*)term_str_tag->term_write;
}
void terminal_Write(const char* string, size_t size)
{
    terminal_write(string, size);
}