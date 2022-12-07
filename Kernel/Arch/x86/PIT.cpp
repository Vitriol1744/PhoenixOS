#include "PIT.hpp"

#include "Arch/x86/IDT.hpp"
#include "Arch/x86/IO.hpp"
#include "Arch/x86/PIC.hpp"

inline static constexpr const uint32_t PIT_BASE_FREQUENCY = 1193182;

static __attribute__((interrupt)) void TimerTick(void*)
{
    PIC::SendEOI(0);
    static uint32_t i = 0;
    // LogError("tick %i\n", i++);
}
namespace PIT
{
    void Initialize() { SetFrequency(9000); }
    void SetFrequency(size_t frequency)
    {
        uint64_t reloadValue = PIT_BASE_FREQUENCY / frequency;
        if (PIT_BASE_FREQUENCY % frequency > frequency / 2) { reloadValue++; }
        IO::Out<byte>(0x40, reloadValue);
        IO::Out<byte>(0x40, reloadValue >> 8);
        IO::Out<byte>(0x43, 0x36);

        // IDT::RegisterInterruptHandler(
        //     0x20, reinterpret_cast<uintptr_t>(TimerTick), 0x8e);
        // PIC::UnmaskIRQ(0);
    }
} // namespace PIT