#include "idt.h"

#include "kernel/memory/gdt.h"
#include "logger.h"

void idtSetDescriptor(uint8_t index, uintptr_t isr, uint8_t flags);
void idtLoad(idt_entry_t* entries, uint8_t number_of_entries);
__attribute__((noreturn)) void defaultExceptionHandler(void);
idt_entry_t                    idt[32];

extern void*                   isr_stub_table[];
void                           idtInitialize()
{
    for (uint8_t index; index < 32; index++)
        idtSetDescriptor(index, isr_stub_table[index], 0x8E);
    idtLoad(idt, 32);
}

void idtSetDescriptor(uint8_t index, uintptr_t isr, uint8_t flags)
{
    idt_entry_t* entry = &(idt[index]);

    entry->isr_low     = isr & 0xFFFF;
    entry->kernel_cs   = GDT_KERNEL_CODE_SELECTOR;
    entry->ist         = 0;
    entry->attributes  = flags;
    entry->isr_middle  = (isr & 0xFFFF0000) >> 16;
    entry->isr_high    = (isr & 0xFFFFFFFF00000000) >> 32;
    entry->reserved    = 0;
}

typedef struct
{
    uint16_t size;
    uint64_t pointer;
} __attribute__((packed)) idt_descriptor_t;

extern void interrupt();
void        idtLoad(idt_entry_t* entries, uint8_t number_of_entries)
{
    PH_LOG_INFO("Loading IDT...");
    idt_descriptor_t idt_descriptor;
    idt_descriptor.size    = sizeof(idt_entry_t) * number_of_entries - 1;
    idt_descriptor.pointer = (uintptr_t)entries;

    PH_ASM_VOL("lidt %0\n" : : "m"(idt_descriptor));
    PH_LOG_INFO("IDT loaded!");
    interrupt();
}

void defaultExceptionHandler()
{
    PH_LOG_INFO("Interrupt Handled!");
    PH_ASM_VOL("cli\nhlt");
}