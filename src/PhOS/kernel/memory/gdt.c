#include "gdt.h"

#include "string.h"

#include "kernel/terminal.h"

extern terminal_t terminal;

extern loadGDT(gdt_t* gdt_ptr, size_t gdt_size);

#define CREATE_GDT_ENTRY(gdt_entry, base, limit, _flags, _access)\
    gdt_entry.limit_low     = limit & 0xFFFF;\
    gdt_entry.base_low      = base  & 0xFFFF;\
    gdt_entry.base_middle   = base  & 0xFF0000;\
    gdt_entry.access        = _access;\
    gdt_entry.limit_high    = limit & 0xF0000;\
    gdt_entry.flags         = _flags;\
    gdt_entry.base_high     = base & 0xFF000000;

void gdt_create(gdt_t* this)
{
    gdt_entry_t* gdt = (gdt_entry_t*)this;

    memset(gdt, 0, sizeof(gdt_entry_t));   
    CREATE_GDT_ENTRY(gdt[1], 0x00000000, 0xFFFFF, 0b1100, 0b10011010);
    CREATE_GDT_ENTRY(gdt[2], 0x00000000, 0xFFFFF, 0b1100, 0b10010110);
    CREATE_GDT_ENTRY(gdt[3], 0x00000000, 0xFFFFF, 0b1100, 0b11111010);
    CREATE_GDT_ENTRY(gdt[4], 0x00000000, 0xFFFFF, 0b1100, 0b11110110);

    loadGDT(this, sizeof(gdt_t));
    
    terminalWrite(&terminal, "gdt loaded!", sizeof("gdt loaded!"));
}