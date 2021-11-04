#include "kernel/io/serial.h"
#include "terminal.h"

#include "kernel/interrupts/idt.h"
#include "kernel/interrupts/pic.h"
#include "kernel/logger.h"
#include "kernel/memory/gdt.h"
#include "stdlib.h"
#include "string.h"

void kernelMain(stivale2_struct_t* bootloader_data)
{
    const char str[] = "PhoenixOS!\n";

    terminalInitialize(bootloader_data);
    terminalWrite(str, sizeof(str));
    PH_LOG_INFO("PhoenixOS is Booting...");
    serialInitialize();

    gdtInitialize();
    PH_LOG_INFO("GDT loaded successfully!");
    picRemap(0x20, 0x28);
    idtInitialize();

    stivale2_struct_tag_memmap_t* memory_map_tag
        = stivale2GetTag(bootloader_data, STIVALE2_STRUCT_TAG_MEMMAP_ID);

    PH_UNUSED(memory_map_tag);

    // outb(0x21, 0xfd);
    // outb(0xa1, 0xff);
    // PH_ASM("sti");

    // char c_standard[20];

    // itoa(__STDC_VERSION__, c_standard, 10);
    // for (int i = 0; i < 20; i++)
    // {
    //     if (c_standard[i] == '\0') break;
    //     serialSendb(COM1, c_standard[i]);
    // }

    for (;;) PH_ASM("hlt");
}