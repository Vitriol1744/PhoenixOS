#include "Arch/Arch.hpp"

#include "Arch/x86/IDT.hpp"
#include "Arch/x86/PIC.hpp"

namespace Arch
{
    static IDT s_IDT;

    void       Initialize()
    {
        s_IDT.Initialize();
        IDT::Load(&s_IDT);
        PIC::MaskAllIRQs();
    }
} // namespace Arch