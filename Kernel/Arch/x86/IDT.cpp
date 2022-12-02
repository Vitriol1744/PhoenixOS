#include "IDT.hpp"

#include "Arch/x86/GDT.hpp"

#include "Common.hpp"

#include <stdnoreturn.h>

#pragma pack(push, 1)
struct IDTR
{
    uint16_t  limit;
    uintptr_t base;
};
#pragma pack(pop)

alignas(0x10) IDTEntry entries[MAX_IDT_ENTRIES];

#pragma region exception_names
const char*    exceptionNames[] = {
       "Divide-by-zero",
       "Debug",
       "Non-Maskable Interrupt",
       "Breakpoint",
       "Overflow",
       "Bound Range Exceeded",
       "Invalid Opcode",
       "Device not available",
       "Double Fault",
       "Coprocessor Segment Overrun",
       "Invalid TSS",
       "Segment Not Present",
       "Stack-Segment Fault",
       "General Protection Fault",
       "Page Fault",
       "Reserved",
       "x87 Floating-Point Exception",
       "Alignment Check",
       "Machine Check",
       "SIMD Floating-Point Exception",
       "Virtualization Exception",
       "Control Protection Exception",
       "Reserved",
       "Reserved",
       "Reserved",
       "Reserved",
       "Reserved",
       "Reserved",
       "Hypervisor Injection Exception",
       "VMM Communication Exception",
       "Security Exception",
       "Reserved",
       "Triple Fault",
       "FPU Error Interrupt",
};
#pragma endregion

using ExceptionHandler = void (*)();
extern "C" ExceptionHandler exception_handlers[32];

extern "C" void raiseException(uint32_t exceptionVector, uint32_t errorCode,
                               uintptr_t rip)
{
    panic("Captured exception: '%s'\nError Code: %d\nrip: %#p",
          exceptionNames[exceptionVector], errorCode, rip);
}
struct InterruptFrame;
__attribute__((interrupt)) void unhandledInterrupt(InterruptFrame*);

void                            IDT::Initialize()
{
    for (uint32_t vector = 0; vector < 32; vector++)
        RegisterInterruptHandler(
            vector, reinterpret_cast<uintptr_t>(exception_handlers[vector]),
            0x8e);
    for (uint32_t vector = 32; vector < MAX_IDT_ENTRIES; vector++)
        RegisterInterruptHandler(
            vector, reinterpret_cast<uintptr_t>(unhandledInterrupt), 0x8e);
}
namespace IDT
{
    void RegisterInterruptHandler(uint8_t vector, uintptr_t isr, uint8_t flags)
    {
        IDTEntry* entry   = entries + vector;
        entry->isrLow     = isr & 0xffff;
        entry->kernelCS   = GDT_KERNEL_CODE_SELECTOR64;
        entry->attributes = flags;
        entry->reserved   = 0;
#if PH_ARCH == PH_ARCH_X86_64
        entry->ist       = 0;
        entry->isrMiddle = (isr & 0xffff0000) >> 16;
        entry->isrHigh   = (isr & 0xffffffff00000000) >> 32;
#elif PH_ARCH == PH_ARCH_IA32
        entry->isrHigh = isr >> 16;
#endif
    }

    void Load()
    {
        IDTR idtr  = {};
        idtr.limit = sizeof(entries) - 1;
        idtr.base  = reinterpret_cast<uintptr_t>(entries);

        __asm__ volatile("lidt %0" : : "m"(idtr));
    }
} // namespace IDT
#if PH_ARCH == PH_ARCH_X86_64
struct InterruptFrame
{
    uint64_t interruptNumber;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};
// TODO: Using int thunk might be a good idea
#elif PH_ARCH == PH_ARCH_IA32
struct InterruptFrame
{
    uint32_t rip;
    uint32_t cs;
    uint32_t rflags;
};
#endif

__attribute__((interrupt)) void
unhandledInterrupt(InterruptFrame* interruptFrame)
{
    LogWarn("Unhandled interrupt has occured! rip: %#p\n", interruptFrame->rip);
    __asm__("cli;hlt");
}