#include "IDT.hpp"

#include "Arch/x86/GDT.hpp"

#include "Utility/Logger.hpp"

#include <stdnoreturn.h>

#pragma pack(push, 1)
struct IDTR
{
    uint16_t  limit;
    uintptr_t base;
};
#pragma pack(pop)

__attribute__((interrupt)) void unhandledInterrupt(void*);

void                            IDT::Initialize()
{
    for (uint32_t vector = 0; vector < MAX_IDT_ENTRIES; vector++)
        RegisterInterruptHandler(
            vector, reinterpret_cast<uintptr_t>(unhandledInterrupt), 0x8e);
}

void IDT::RegisterInterruptHandler(uint8_t vector, uintptr_t isr, uint8_t flags)
{
    IDTEntry* entry   = entries + vector;
    entry->isrLow     = isr & 0xffff;
    entry->kernelCS   = 0x28;
    entry->ist        = 0;
    entry->attributes = flags;
    entry->isrMiddle  = (isr & 0xffff0000) >> 16;
    entry->isrHigh    = (isr & 0xffffffff00000000) >> 32;
    entry->reserved   = 0;
}

void IDT::Load(IDT* idt)
{
    IDTR idtr  = {};
    idtr.limit = sizeof(idt->entries) - 1;
    idtr.base  = reinterpret_cast<uintptr_t>(idt->entries);

    __asm__ volatile("lidt %0" : : "m"(idtr));
    __asm__ volatile("sti");
}

__attribute__((interrupt)) void unhandledInterrupt(void*)
{
    LogWarn("Unhandled interrupt has occured!\n");
    __asm__("cli; hlt");
}