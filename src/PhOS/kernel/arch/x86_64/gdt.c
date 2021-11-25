#include "gdt.h"

#include "common.h"
#include "kernel/logger.h"

#include "string.h"

#define CREATE_GDT_ENTRY(entry, base, limit, _access, _flags)                  \
    entry.limit_low   = limit & 0xFFFF;                                        \
    entry.base_low    = base & 0xFFFF;                                         \
    entry.base_middle = (base & 0x00FF0000) >> 16;                             \
    entry.access      = _access;                                               \
    entry.limit_high  = (limit & 0x000F0000) >> 16;                            \
    entry.flags       = _flags;                                                \
    entry.base_high   = (base & 0xFF000000) >> 24;

gdt_t g_GDT;

void  gdt_Initialize()
{
    gdt_entry_t* gdt = (gdt_entry_t*)&g_GDT;

    gdt_access_t kernel_code_access
        = GDT_ACCESS_PRESENT | GDT_ACCESS_RING_0 | GDT_ACCESS_CODE_OR_DATA
        | GDT_ACCESS_CODE_SEGMENT | GDT_ACCESS_READABLE;
    gdt_access_t kernel_data_access
        = GDT_ACCESS_PRESENT | GDT_ACCESS_RING_0 | GDT_ACCESS_CODE_OR_DATA
        | GDT_ACCESS_DATA_SEGMENT | GDT_ACCESS_GROWS_UP | GDT_ACCESS_WRITEABLE;
    gdt_access_t userland_code_access
        = GDT_ACCESS_PRESENT | GDT_ACCESS_RING_3 | GDT_ACCESS_CODE_OR_DATA
        | GDT_ACCESS_CODE_SEGMENT | GDT_ACCESS_READABLE;
    gdt_access_t userland_data_access = GDT_ACCESS_PRESENT | GDT_ACCESS_RING_3
                                      | GDT_ACCESS_CODE_OR_DATA
                                      | GDT_ACCESS_WRITEABLE;

    gdt_flag_t kernel_code_flag
        = GDT_FLAG_GRANULARITY_4K | GDT_FLAG_64BIT_SEGMENT;
    gdt_flag_t kernel_data_flag
        = GDT_FLAG_GRANULARITY_4K | GDT_FLAG_32BIT_SEGMENT;
    gdt_flag_t userland_code_flag = kernel_code_flag;
    gdt_flag_t userland_data_flag = kernel_data_flag;

    // null segment
    memset(gdt, 0, sizeof(gdt_entry_t));
    // kernel code segment
    CREATE_GDT_ENTRY(gdt[1], 0x00000000, 0xFFFFF, kernel_code_access,
                     kernel_code_flag);
    // kernel data segment
    CREATE_GDT_ENTRY(gdt[2], 0x00000000, 0xFFFFF, kernel_data_access,
                     kernel_data_flag);
    // userland code segment
    CREATE_GDT_ENTRY(gdt[3], 0x00000000, 0xFFFFF, userland_code_access,
                     userland_code_flag);
    // userland data segment
    CREATE_GDT_ENTRY(gdt[4], 0x00000000, 0xFFFFF, userland_data_access,
                     userland_data_flag);
    gdt_Load();
}

typedef struct
{
    uint16_t size;
    uint64_t pointer;
} __attribute__((packed)) gdt_descriptor_t;

void gdt_Load()
{
    gdt_descriptor_t gdt_descriptor;
    gdt_descriptor.size    = sizeof(gdt_t) - 1;
    gdt_descriptor.pointer = (uintptr_t)(&g_GDT);

    PH_LOG_INFO("Loading GDT...");
    PH_ASM_VOL(
        "cli\n"
        "lgdt %0\n"
        "pushq %%rbp\n"
        "movq %%rsp, %%rbp\n"
        "pushq %2\n"
        "pushq %%rbp\n"
        "pushfq\n"
        "pushq %1\n"
        "pushq $1f\n"
        "iretq\n"
        "1:\n"
        "popq %%rbp\n"
        "mov %%ds, %2\n"
        "mov %%es, %2\n"
        "mov %%fs, %2\n"
        "mov %%gs, %2\n"
        "mov %%ss, %2\n"
        :
        : "m"(gdt_descriptor), "r"((uint64_t)(GDT_KERNEL_CODE_SELECTOR)),
          "r"((uint64_t)(GDT_KERNEL_DATA_SELECTOR))
        : "memory");
}