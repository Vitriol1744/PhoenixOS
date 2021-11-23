#include "apic.h"

#include "kernel/cpu.h"

#define IA32_APIC_BASE_MSR        0x1B
#define IA32_APIC_BASE_MSR_BSP    0x100
#define IA32_APIC_BASE_MSR_ENABLE 0x800

bool apicCheckSupport(void)
{
    dword_t eax, edx;
    cpuid(1, &eax, &edx);
    return edx & CPUID_FEAT_EDX_APIC;
}
void apicEnable(void)
{
    // TODO: Enable APIC!
}