#pragma once

#include "Arch/Arch.hpp"

#include <stdint.h>

inline constexpr const uint32_t MAX_IDT_ENTRIES = 256;

#pragma pack(push, 1)
struct IDTEntry
{
    uint16_t isrLow;
    uint16_t kernelCS;
#if PH_ARCH == PH_ARCH_X86_64
    uint8_t ist;
#elif PH_ARCH == PH_ARCH_IA32
    uin8_t   reserved;
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

class IDT
{
  public:
    void Initialize();
    void RegisterInterruptHandler(uint8_t vector, uintptr_t isr, uint8_t flags);

    static void Load(IDT* idt);

  private:
    alignas(0x10) IDTEntry entries[MAX_IDT_ENTRIES];
};