#include "gdt.h"

#include "common.h"

#define CREATE_GDT_ENTRY(entry, base, limit, _access, _flags)                  \
    entry.limit_low = limit & 0xFFFF;                                          \
    entry.base_low = base & 0xFFFF;                                            \
    entry.base_middle = (base & 0x00FF0000) >> 16;                             \
    entry.access = _access;                                                    \
    entry.limit_high = (limit & 0xF0000) >> 16;                                \
    entry.flags = _flags;                                                      \
    entry.base_high = (base & 0xFF000000) >> 24;

void createGDT(gdt_t* _gdt)
{
    gdt_entry_t* gdt = (gdt_entry_t*)_gdt;

    // null segment
    memset(gdt, 0, sizeof(gdt_entry_t));
    // kernel code segment
    CREATE_GDT_ENTRY(gdt[1], 0x00000000, 0xFFFFF, 0x9A, 0xA);
    // kernel data segment
    CREATE_GDT_ENTRY(gdt[2], 0x00000000, 0xFFFFF, 0x92, 0xC);
    // userland code segment
    CREATE_GDT_ENTRY(gdt[3], 0x00000000, 0xFFFFF, 0xFA, 0xC);
    // userland data segment
    CREATE_GDT_ENTRY(gdt[4], 0x00000000, 0xFFFFF, 0xF2, 0xC);
}

typedef struct
{
    uint16_t size;
    uint64_t pointer;
} __attribute__((packed)) gdt_descriptor_t;

void loadGDT(gdt_t* gdt)
{
    gdt_descriptor_t gdt_descriptor;
    gdt_descriptor.size = sizeof(gdt_t) - 1;
    gdt_descriptor.pointer = (uintptr_t)(gdt);

    PH_ASM_VOL("cli\n"
               "lgdt %0\n"
               "pushq %%rbp\n"
               "movq %%rsp, %%rbp\n"
               "pushq %1\n"
               "pushq %%rbp\n"
               "pushfq\n"
               "pushq %2\n"
               "pushq $1f\n"
               "iretq\n"
               "1:\n"
               "popq %%rbp\n"
               "mov %%ds, %1\n"
               "mov %%es, %1\n"
               "mov %%fs, %1\n"
               "mov %%gs, %1\n"
               "mov %%ss, %1\n"
               :
               : "m"(gdt_descriptor), "r"((uint64_t)(0x10)),
                 "r"((uint64_t)(0x08))
               : "memory");
}