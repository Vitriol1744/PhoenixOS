#ifndef GDT_H
#define GDT_H

#include "common.h"

PH_CONSTEXPR(GDT_KERNEL_CODE_SELECTOR, 0x08);
PH_CONSTEXPR(GDT_KERNEL_DATA_SELECTOR, 0x10);

typedef enum
{
    GDT_ACCESS_PRESENT             = 1 << 7,
    GDT_ACCESS_RING_0              = 0,
    GDT_ACCESS_RING_3              = 1 << 6 | 1 << 5,
    GDT_ACCESS_CODE_OR_DATA        = 1 << 4,
    GDT_ACCESS_CODE_SEGMENT        = 1 << 3,
    GDT_ACCESS_DATA_SEGMENT        = 0,
    GDT_ACCESS_GROWS_UP            = 0,
    GDT_ACCESS_GROWS_DOWN          = 1 << 2,
    GDT_ACCESS_EXE_FROM_LOWER_RING = 1 << 2,
    GDT_ACCESS_READABLE            = 1 << 1,
    GDT_ACCESS_WRITEABLE           = 1 << 1,

} gdt_access_t;

typedef enum
{
    GDT_FLAG_GRANULARITY_1B = 0,
    GDT_FLAG_GRANULARITY_4K = 1 << 3,
    GDT_FLAG_16BIT_SEGMENT  = 0,
    GDT_FLAG_32BIT_SEGMENT  = 1 << 2,
    GDT_FLAG_64BIT_SEGMENT  = 1 << 1
} gdt_flag_t;

typedef struct
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  limit_high : 4;
    uint8_t  flags      : 4;
    uint8_t  base_high;

} __attribute__((packed)) gdt_entry_t;

typedef struct
{
    gdt_entry_t null_segment;
    gdt_entry_t kernel_code_segment;
    gdt_entry_t kernel_data_segment;
    gdt_entry_t userland_code_segment;
    gdt_entry_t userland_data_segment;
    // TODO: Add some more segments e.g tss
} __attribute__((packed)) gdt_t;

void gdtInitialize(void);
void gdtLoad();

#endif // GDT_H