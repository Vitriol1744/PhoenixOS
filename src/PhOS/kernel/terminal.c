#include "terminal.h"

void terminalInitialize(terminal_t* this, stivale2_struct_t* bootloader_data)
{
    struct stivale2_struct_tag_terminal* term_str_tag =
        stivale2GetTag(bootloader_data, STIVALE2_STRUCT_TAG_TERMINAL_ID);

    if (!term_str_tag)
    {
        for (;;)
            PH_ASM("hlt");
    }

    this->terminalWrite = (void*)term_str_tag->term_write;
}
void terminalWrite(terminal_t* this, const char* string, size_t size)
{
    this->terminalWrite(string, size);
}