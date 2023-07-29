/*
 * Created by vitriol1744 on 16.07.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "IOAPIC.hpp"

#include "ACPI/MADT.hpp"
#include "Memory/VirtualMemoryManager.hpp"

namespace IOAPIC
{
    namespace
    {
        void Write(MADT::IOAPICEntry* ioApic, u32 reg, u32 value)
        {
            u64 base = ToHigherHalfAddress<u64>(ioApic->address);
            *reinterpret_cast<volatile u32*>(base)      = reg;
            *reinterpret_cast<volatile u32*>(base + 16) = value;
        }
        u32 Read(MADT::IOAPICEntry* ioApic, u32 reg)
        {
            u64 base = ToHigherHalfAddress<u64>(ioApic->address);
            *reinterpret_cast<volatile u32*>(base) = reg;
            return *reinterpret_cast<volatile u32*>(base + 16);
        }

        usize GetGSI_Count(MADT::IOAPICEntry* ioApic)
        {
            return (Read(ioApic, 1) & 0xff0000) >> 16;
        }

        MADT::IOAPICEntry* IOApicFromGSI(u32 gsi)
        {
            std::vector<MADT::IOAPICEntry*> ioApics = MADT::GetIOAPICEntries();
            for (usize i = 0; i < ioApics.size(); i++)
            {
                MADT::IOAPICEntry* ioApic = ioApics[i];
                if (gsi >= ioApic->gsib
                    && gsi < ioApic->gsib + GetGSI_Count(ioApic))
                    return ioApic;
            }
            Panic("Cannot determine IO APIC from GSI {}", gsi);
        }
    } // namespace

    void SetIRQRedirect(u32 lapicID, u8 vector, u8 irq, bool status)
    {
        std::vector<MADT::ISOEntry*> isos = MADT::GetISOEntries();
        for (usize i = 0; i < isos.size(); i++)
        {
            MADT::ISOEntry* iso = isos[i];
            if (iso->irqSource != irq) continue;

            SetGSIRedirect(lapicID, vector, iso->gsi, iso->flags, status);
            return;
        }

        SetGSIRedirect(lapicID, vector, irq, 0, status);
    }
    void SetGSIRedirect(u32 lapicID, u8 vector, u8 gsi, u16 flags, bool status)
    {
        MADT::IOAPICEntry* ioApic   = IOApicFromGSI(gsi);

        u64                redirect = vector;
        if ((flags & (1 << 1)) != 0) { redirect |= (1 << 13); }

        if ((flags & (1 << 3)) != 0) { redirect |= (1 << 15); }

        if (!status) { redirect |= (1 << 16); }

        redirect |= static_cast<u64>(lapicID) << 56;

        u32 ioRedirectTable = (gsi - ioApic->gsib) * 2 + 16;
        Write(ioApic, ioRedirectTable, static_cast<u32>(redirect));
        Write(ioApic, ioRedirectTable + 1, static_cast<u32>(redirect >> 32));
    }
} // namespace IOAPIC