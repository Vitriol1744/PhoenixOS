/*
 * Created by vitriol1744 on 17.07.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "ACPI.hpp"

#include "BootInfo.hpp"

namespace ACPI
{
    namespace
    {
#pragma pack(push, 1)
        struct RSDP
        {
            char signature[8];
            u8   checksum;
            char oemID[6];
            u8   revision;
            u32  rsdtAddress;
            u32  length;
            u64  xsdtAddress;
            u8   extendedChecksum;
            u8   reserved[3];
        };
        struct RSDT
        {
            SDTHeader header;
            char      sdts[];
        };
#pragma pack(pop)

        bool  xsdt = false;
        RSDT* rsdt;

        bool  ValidateChecksum(SDTHeader* header)
        {
            u32 checksum = 0;
            for (u32 i = 0; i < header->length; i++)
                checksum += reinterpret_cast<char*>(header)[i];

            return checksum;
        }
        uintptr_t GetTablePointer(u8 index)
        {
            if (xsdt)
            {
                const u64* ptr = reinterpret_cast<u64*>(rsdt->sdts);

                return ptr[index] + BootInfo::GetHHDMOffset();
            }

            const u32* ptr = reinterpret_cast<u32*>(rsdt->sdts);
            return static_cast<uintptr_t>(ptr[index])
                 + BootInfo::GetHHDMOffset();
        }
        void PrintACPITables()
        {
            const usize entryCount
                = (rsdt->header.length - sizeof(SDTHeader)) / 4;
            LogInfo("ACPI: Tables count: {}", entryCount);

            char acpiSignature[5];
            acpiSignature[4] = '\0';

            LogTrace("ACPI: Printing all tables signatures...");
            for (usize i = 0; i < entryCount; i++)
            {
                SDTHeader* header
                    = reinterpret_cast<SDTHeader*>(GetTablePointer(i));
                if (!header || !((char*)header->signature)
                    || header->signature[0] == 0)
                    continue;
                memcpy(acpiSignature, header->signature, 4);
                LogTrace("  -{}", acpiSignature);
            }
        }
    } // namespace

    void Initialize()
    {
        RSDP* rsdp      = reinterpret_cast<RSDP*>(BootInfo::GetRSDPAddress());
        xsdt            = rsdp->revision == 2 && rsdp->xsdtAddress != 0;

        u64 rsdtPointer = xsdt ? rsdp->xsdtAddress : rsdp->rsdtAddress;
        rsdt = reinterpret_cast<RSDT*>(rsdtPointer + BootInfo::GetHHDMOffset());
        Assert(rsdt != nullptr);
        PrintACPITables();

        LogTrace("ACPI: Initialized");
    }
    SDTHeader* GetTable(const char* signature, usize index)
    {
        Assert(signature != nullptr);

        usize       count      = 0;
        const usize entryCount = (rsdt->header.length - sizeof(SDTHeader)) / 4;
        for (usize i = 0; i < entryCount; i++)
        {
            SDTHeader* header
                = reinterpret_cast<SDTHeader*>(GetTablePointer(i));

            if (!header || !((char*)header->signature)) continue;
            if (!ValidateChecksum(header)) continue;

            if (strncmp(header->signature, signature, 4) == 0) return header;
        }

        LogWarn("ACPI: Failed to find {} table", signature);
        return nullptr;
    }
} // namespace ACPI