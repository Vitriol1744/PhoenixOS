#include "terminal.h"
#include "HAL/serial.h"

#include "stdlib.h"
#include "logger.h"
#include "memory/gdt.h"

terminal_t terminal;

void kernelMain(stivale2_struct_t* bootloader_data)
{
    const char* str = "phoenixOS!\n";
    
    terminalInitialize(&terminal, bootloader_data);
    terminalWrite(&terminal, str, 11);
    serialInitialize();
    
    serialSendb(COM1, 'H');
    serialSendb(COM1, 'e');
    serialSendb(COM1, 'l');
    serialSendb(COM1, 'l');
    serialSendb(COM1, 'o');
    serialSendb(COM1, ',');
    serialSendb(COM1, ' ');
    serialSendb(COM1, 'W');
    serialSendb(COM1, 'o');
    serialSendb(COM1, 'r');
    serialSendb(COM1, 'l');
    serialSendb(COM1, 'd');
    serialSendb(COM1, '!');
    serialSendb(COM1, '\n');
    
    gdt_t* gdt;
    //gdt_create(gdt);
    terminalWrite(&terminal, "gdt loaded!", sizeof("gdt loaded!"));

    PH_LOG_TRACE("Hello, %s!", "World");
    
    char c_standard[20];

    itoa(__STDC_VERSION__, c_standard, 10);
    for (int i = 0; i < 20; i++)
    {
        if (c_standard[i] == '\0') break;
        serialSendb(COM1, c_standard[i]);
    }

    for (;;) PH_ASM("hlt");
}