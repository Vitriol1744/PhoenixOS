#include "HAL/serial.h"
#include "terminal.h"

#include "interrupts/idt.h"
#include "logger.h"
#include "memory/gdt.h"
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
    idtInitialize();

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