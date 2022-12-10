#include "PIT.hpp"

#include "Arch/x86/IDT.hpp"
#include "Arch/x86/IO.hpp"
#include "Arch/x86/PIC.hpp"

static __attribute__((interrupt)) void TimerTick(void*)
{
    PIC::SendEOI(0);
    static uint32_t i = 0;
    // LogError("tick %i\n", i++);
}
namespace PIT
{
    void Initialize() { SetFrequency(100); }
    void SetFrequency(size_t frequency)
    {
        uint64_t reloadValue = PIT_BASE_FREQUENCY / frequency;
        if (PIT_BASE_FREQUENCY % frequency > frequency / 2) { reloadValue++; }
        SetReloadValue(reloadValue);
    }
    void SetReloadValue(uint16_t reloadValue)
    {
        IO::Out<byte>(0x43, 0x34);
        IO::Out<byte>(0x40, static_cast<uint8_t>(reloadValue));
        IO::Out<byte>(0x40, static_cast<uint8_t>(reloadValue >> 8));
    }
    uint64_t GetCurrentCount()
    {
        IO::Out<byte>(0x43, 0x00);
        uint8_t lo = IO::In<byte>(0x40);
        uint8_t hi = IO::In<byte>(0x40) << 8;
        return static_cast<uint16_t>(hi << 8) | lo;
    }
} // namespace PIT