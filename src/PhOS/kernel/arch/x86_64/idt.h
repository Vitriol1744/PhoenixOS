#ifndef IDT_H
#define IDT_H

#include "common.h"

#define IDT_MAX_ENTRIES 256

typedef struct
{
    uint16_t isr_low;
    uint16_t kernel_cs;
    uint8_t  ist;
    uint8_t  attributes;
    uint16_t isr_middle;
    uint32_t isr_high;
    uint32_t reserved;
} __attribute__((packed)) idt_entry_t;

extern void idt_Initialize(void);

#endif // IDT_H