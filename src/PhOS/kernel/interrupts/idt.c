#include "idt.h"

#include "kernel/io/io_access.h"
#include "kernel/logger.h"
#include "kernel/memory/gdt.h"

void idtSetDescriptor(uint8_t index, uintptr_t isr, uint8_t flags);
void idtLoad(idt_entry_t* entries, uint8_t number_of_entries);
__attribute__((noreturn)) void             defaultExceptionHandler(void);
__attribute__((aligned(0x10))) idt_entry_t g_IDT[32];

extern void*                               isr_stub_table[];
void                                       idtInitialize()
{
    for (uint8_t index = 0; index < 32; index++)
        idtSetDescriptor(index, (uintptr_t)isr_stub_table[index], 0x8E);
    idtLoad(g_IDT, 32);
}

void idtSetDescriptor(uint8_t interrupt_vector, uintptr_t isr, uint8_t flags)
{
    idt_entry_t* entry = g_IDT + interrupt_vector;

    entry->isr_low     = isr & 0xffff;
    entry->kernel_cs   = GDT_KERNEL_CODE_SELECTOR;
    entry->ist         = 0;
    entry->attributes  = flags;
    entry->isr_middle  = (isr & 0xffff0000) >> 16;
    entry->isr_high    = (isr & 0xffffffff00000000) >> 32;
    entry->reserved    = 0;
}

typedef struct
{
    uint16_t size;
    uint64_t pointer;
} __attribute__((packed)) idt_descriptor_t;

void idtLoad(idt_entry_t* entries, uint8_t number_of_entries)
{
    PH_LOG_INFO("Loading IDT...");
    idt_descriptor_t idt_descriptor;
    idt_descriptor.size    = sizeof(idt_entry_t) * number_of_entries - 1;
    idt_descriptor.pointer = (uintptr_t)entries;

    PH_ASM_VOL("lidt %0\n" : : "m"(idt_descriptor));
    PH_LOG_INFO("IDT loaded!");

    outb(0x21, 0xfd);
    outb(0xa1, 0xff);
    PH_ASM("sti");
}

void defaultExceptionHandler()
{
    PH_LOG_INFO("Interrupt Handled!");
    PH_ASM_VOL("cli");
    while (true) PH_ASM_VOL("hlt");
}