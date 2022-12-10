#include "IDT.hpp"

#include "Common.hpp"

#include "Arch/x86/CPU.hpp"
#include "Arch/x86/GDT.hpp"
#include "Arch/x86/LAPIC.hpp"

#include <stdint.h>
#include <stdnoreturn.h>

extern const char* exceptionNames[];

static void        raiseException(CPUContext* cpuContext)
{
    panic("Captured exception on cpu %d: '%s'\nError Code: %#b\nrip: %#p",
          (uint32_t)CPU::GetCurrentID(),
          exceptionNames[cpuContext->interruptVector], cpuContext->errorCode,
          cpuContext->rip);
}

inline constexpr const uint32_t MAX_IDT_ENTRIES   = 256;

inline constexpr const uint32_t IDT_ENTRY_PRESENT = BIT(7);

#pragma pack(push, 1)
struct IDTEntry
{
    uint16_t isrLow;
    uint16_t kernelCS;
#if PH_ARCH == PH_ARCH_X86_64
    uint8_t ist;
#elif PH_ARCH == PH_ARCH_IA32
    uint8_t  reserved;
#endif
    union
    {
        uint8_t attributes;
        struct
        {
            uint8_t gateType : 4;
            uint8_t unused   : 1;
            uint8_t dpl      : 2;
            uint8_t present  : 1;
        };
    };
#if PH_ARCH == PH_ARCH_X86_64
    uint16_t isrMiddle;
    uint32_t isrHigh;
    uint32_t reserved;
#elif PH_ARCH == PH_ARCH_IA32
    uint16_t isrHigh;
#endif
};
#pragma pack(pop)

inline constexpr const uint32_t GATE_TYPE_INTERRUPT = 0xe;
inline constexpr const uint32_t GATE_TYPE_TRAP      = 0xf;

alignas(0x10) static IDTEntry idtEntries[256]       = {0};
static InterruptHandler interruptHandlers[256]      = {0};

extern "C" void*        interrupt_handlers[];

static void             idtWriteEntry(uint16_t vector, uintptr_t handler,
                                      uint8_t attributes)
{
    Assert(vector <= 256);

    IDTEntry* entry   = idtEntries + vector;

    entry->isrLow     = handler & 0xffff;
    entry->kernelCS   = KERNEL_CODE_SELECTOR;
    entry->attributes = attributes;
    entry->reserved   = 0;
#if PH_ARCH == PH_ARCH_X86_64
    entry->ist       = 0;
    entry->isrMiddle = (handler & 0xffff0000) >> 16;
    entry->isrHigh   = (handler & 0xffffffff00000000) >> 32;
#elif PH_ARCH == PH_ARCH_IA32
    entry->isrHigh = isr >> 16;
#endif
}

static void unhandledInterrupt(CPUContext* context)
{
    if (context->interruptVector < 0x20) raiseException(context);
    LogError("\nAn unhandled interrupt 0x%02x occured",
             context->interruptVector);

    for (;;) { __asm__ volatile("cli; hlt"); }
}

extern "C" void raiseInterrupt(CPUContext* context)
{
    interruptHandlers[context->interruptVector](context);
    LAPIC::SendEOI();
}

namespace IDT
{
    void Initialize()
    {
        for (uint32_t i = 0; i < 256; i++)
        {
            idtWriteEntry(i, reinterpret_cast<uintptr_t>(interrupt_handlers[i]),
                          0x80 | GATE_TYPE_INTERRUPT);
            interruptHandlers[i] = unhandledInterrupt;
        }

        LogInfo("IDT Initialized!");
    }

    void Load()
    {
#pragma pack(push, 1)
        struct
        {
            uint16_t  limit;
            uintptr_t base;
        } idtr;
#pragma pack(pop)
        idtr.limit = sizeof(idtEntries) - 1;
        idtr.base  = reinterpret_cast<uintptr_t>(idtEntries);
        __asm__ volatile("lidt %0" : : "m"(idtr));
    }

    void RegisterInterruptHandler(uint32_t vector, InterruptHandler handler,
                                  uint8_t dpl)
    {
        Assert(vector <= 256);
        interruptHandlers[vector] = handler;
        idtEntries[vector].dpl    = dpl;
    }

} // namespace IDT

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
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "",
    "Virtualization Exception",
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
