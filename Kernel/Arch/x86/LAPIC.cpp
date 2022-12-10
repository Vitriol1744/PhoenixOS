#include "LAPIC.hpp"

#include "Arch/x86/CPU.hpp"
#include "Arch/x86/HPET.hpp"
#include "Arch/x86/IDT.hpp"
#include "Arch/x86/IO.hpp"
#include "Arch/x86/PIC.hpp"
#include "Arch/x86/PIT.hpp"

#include "Common.hpp"

inline static constexpr const uint32_t APIC_BASE_MSR              = 0x1b;

inline static constexpr const uint32_t LAPIC_ID_REGISTER          = 0x20;
inline static constexpr const uint32_t LAPIC_EOI_REGISTER         = 0xb0;
inline static constexpr const uint32_t LAPIC_SPURIOUS_REGISTER    = 0xf0;
inline static constexpr const uint32_t LAPIC_TIMER_REGISTER       = 0x320;
inline static constexpr const uint32_t LAPIC_TIMER_INITIAL_COUNT_REGISTER
    = 0x380;
inline static constexpr const uint32_t LAPIC_TIMER_CURRENT_COUNT_REGISTER
    = 0x390;
inline static constexpr const uint32_t LAPIC_TIMER_DIVIDER_REGISTER = 0x3e0;

inline static constexpr const uint32_t LAPIC_TIMER_MASKED           = 0x10000;
inline static constexpr const uint32_t LAPIC_TIMER_PERIODIC         = 0x20000;

void                                   Schedule(CPUContext*);

static bool                            IsAPICSupported()
{
    uint64_t rax, unused, rdx;
    CPU::ID(1, rax, unused, unused, rdx);
    return rdx & CPU_FEAT_EDX_APIC;
}

namespace LAPIC
{
    static void InitializeTimer();

    static void Write(uint32_t reg, uint32_t value)
    {
        auto address = reinterpret_cast<uint8_t*>(
            CPU::ReadMSR(APIC_BASE_MSR) & 0xfffff000);
        *(reinterpret_cast<volatile uint32_t*>(address + reg)) = value;
    }

    static uint32_t Read(uint32_t reg)
    {
        auto address = reinterpret_cast<uint8_t*>(
            CPU::ReadMSR(APIC_BASE_MSR) & 0xfffff000);
        return *(reinterpret_cast<volatile uint32_t*>(address + reg));
    }

    void Initialize()
    {
        Assert(IsAPICSupported());
        PIC::MaskAllIRQs();
        InitializeTimer();

        LogInfo("APIC Initialized!");
    }

    void    SendEOI() { Write(LAPIC_EOI_REGISTER, 0); }

    static uint64_t frequency = 0;
    static void     Stop()
    {
        Write(LAPIC_TIMER_INITIAL_COUNT_REGISTER, 0);
        Write(LAPIC_TIMER_REGISTER, 1 << 16);
    }
    void InitializeTimer()
    {
        Assert((CPU::ReadMSR(0x1b) & 0xfffff000) == 0xfee00000);
        Write(0x0f0, Read(0x0f0) | (1 << 8) | 0xff);

        IDT::RegisterInterruptHandler(LAPIC_TIMER_ISR, Schedule);
        Write(LAPIC_TIMER_REGISTER,
              Read(LAPIC_TIMER_REGISTER) | (1 << 8) | LAPIC_TIMER_ISR);
        Stop();
        Write(LAPIC_TIMER_REGISTER, (1 << 16) | 0xff);
        Write(LAPIC_TIMER_DIVIDER_REGISTER, 0);
        PIT::SetReloadValue(0xffff); // Reset PIT

        uint64_t initTick = PIT::GetCurrentCount();
        int samples  = 0xfffff;
        Write(LAPIC_TIMER_INITIAL_COUNT_REGISTER, (uint32_t)samples);
        while (Read(LAPIC_TIMER_CURRENT_COUNT_REGISTER) != 0)
            ;
        uint64_t finalTick  = PIT::GetCurrentCount();
        uint64_t totalTicks = initTick - finalTick;
        frequency      = (samples / totalTicks) * PIT_BASE_FREQUENCY;
        Stop();
    }
    void OneShot(uint64_t us, InterruptHandler handler)
    {
        Stop();
        bool intFlag = CPU::GetInterruptFlag();
        CPU::SetInterruptFlag(false);
        IDT::RegisterInterruptHandler(LAPIC_TIMER_ISR, handler);
        CPU::SetInterruptFlag(intFlag);


        uint32_t ticks = us * (frequency / 1000000);
        Write(LAPIC_TIMER_REGISTER, LAPIC_TIMER_ISR);
        Write(LAPIC_TIMER_DIVIDER_REGISTER, 0);
        Write(LAPIC_TIMER_INITIAL_COUNT_REGISTER, ticks);
    }
} // namespace LAPIC
