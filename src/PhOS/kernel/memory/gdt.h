#ifndef GDT_H
#define GDT_H

#include <stdint.h>

typedef struct
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t limit_high : 4;
    uint8_t flags : 4;
    uint8_t base_high;

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

void createGDT(gdt_t *gdt);
void loadGDT(gdt_t *gdt);

#endif // GDT_H