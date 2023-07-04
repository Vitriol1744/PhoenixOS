/*
 * Created by vitriol1744 on 29.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "IDT.hpp"

#include "Common.hpp"

#include "Arch/x86_64/Drivers/PIC.hpp"
#include "Arch/x86_64/GDT.hpp"

struct CPUContext
{
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;

    uint64_t interruptVector;
    uint64_t errorCode;

    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

extern const char* exceptionNames[];

[[noreturn]]
static void raiseException(CPUContext* cpuContext)
{
    Panic("Captured exception on cpu {}: '{}'\n\rError Code: {}\n\rrip: {}", 0,
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
    uint8_t  ist;
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
    uint16_t isrMiddle;
    uint32_t isrHigh;
    uint32_t reserved;
};
#pragma pack(pop)

inline constexpr const uint32_t GATE_TYPE_INTERRUPT = 0xe;
inline constexpr const uint32_t GATE_TYPE_TRAP      = 0xf;

alignas(0x10) static IDTEntry idtEntries[256]       = {};
static InterruptHandler* interruptHandlers[256]     = {};

extern "C" void*         interrupt_handlers[];

static void              idtWriteEntry(uint16_t vector, uintptr_t handler,
                                       uint8_t attributes)
{
    Assert(vector <= 256);

    IDTEntry* entry   = idtEntries + vector;

    entry->isrLow     = handler & 0xffff;
    entry->kernelCS   = KERNEL_CODE_SELECTOR;
    entry->attributes = attributes;
    entry->reserved   = 0;
    entry->ist        = 0;
    entry->isrMiddle  = (handler & 0xffff0000) >> 16;
    entry->isrHigh    = (handler & 0xffffffff00000000) >> 32;
}

[[noreturn]]
static void unhandledInterrupt(CPUContext* context)
{
    LogError("\nAn unhandled interrupt 0x{:x} occurred",
             context->interruptVector);

    for (;;) { __asm__ volatile("cli; hlt"); }
}

extern "C" void raiseInterrupt(CPUContext* context)
{
    if (context->interruptVector < 0x20) raiseException(context);
    else if (interruptHandlers[context->interruptVector])
    {
        InterruptHandler& handler
            = *interruptHandlers[context->interruptVector];
        handler(context);
        handler.OnEndOfInterrupt();
        return;
    }
    unhandledInterrupt(context);
}

namespace IDT
{
    void Initialize()
    {
        for (uint32_t i = 0; i < 256; i++)
        {
            idtWriteEntry(i, reinterpret_cast<uintptr_t>(interrupt_handlers[i]),
                          0x80 | GATE_TYPE_TRAP);
            interruptHandlers[i] = nullptr;
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

    void RegisterInterruptHandler(uint32_t                 vector,
                                  InterruptHandlerFunction handler, uint8_t dpl)
    {
        Assert(vector <= 256);
        //        interruptHandlers[vector] = handler;
        idtEntries[vector].dpl = dpl;
    }
    void RegisterInterruptHandler(InterruptHandler* handler, uint8_t dpl)
    {
        Assert(handler->GetInterruptVector() >= 0x20);
        interruptHandlers[handler->GetInterruptVector()] = handler;
        idtEntries[handler->GetInterruptVector()].dpl    = dpl;
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
