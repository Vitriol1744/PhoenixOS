/*
 * Created by vitriol1744 on 07.12.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "HPET.hpp"

#include "ACPI.hpp"
#include "Common.hpp"

#pragma pack(push, 1)
struct HPETGeneralCapabilities
{
    uint8_t  revisionID;
    uint8_t  timerCount               : 5;
    uint8_t  _64bitCapable            : 1;
    uint8_t  reserved                 : 1;
    uint8_t  legacyReplacementMapping : 1;
    uint16_t vendorID;
    uint32_t tickPeriod;
};

struct HPETGeneralConfiguration
{
    uint8_t  enabled                        : 1;
    uint8_t  legacyReplacementMappingEnable : 1;
    uint64_t reserved                       : 62;
};

struct HPETEntry
{
    HPETGeneralCapabilities  capabilities;
    uint64_t                 reserved1;
    HPETGeneralConfiguration configuration;
    uint64_t                 reserved2;
    uint64_t                 interruptStatusRegister;
    uint64_t                 reserved3;
    uint64_t                 reserved4[24];
    uint64_t                 counterValue;
    uint64_t                 reserved5;
};

struct HPETEntryAddress
{
    uint8_t  addressSpaceID;
    uint8_t  registerBitWidth;
    uint8_t  registerBitOffset;
    uint8_t  reserved;
    uint64_t base;
};

struct HPETTable
{
    SDTHeader        header;
    uint8_t          hardwareRevisionID;
    uint8_t          comparatorCount          : 5;
    uint8_t          counterSize              : 1;
    uint8_t          reserved                 : 1;
    uint8_t          legacyReplacementMapping : 1;
    uint16_t         pciVendorID;
    HPETEntryAddress address;
    uint8_t          hpetIndex;
    uint16_t         minimumTick;
    uint8_t          pageProtection;
};
#pragma pack(pop)

namespace HPET
{
    static HPETTable* hpetTable  = NULL;
    static HPETEntry* hpetTimer  = NULL;

    static uint64_t   tickPeriod = 0;

    void              Initialize()
    {
        hpetTable = (HPETTable*)ACPI::GetTable("HPET");
        if (!hpetTable) return;

        hpetTimer = (HPETEntry*)hpetTable->address.base;
        if (!hpetTimer) return;

        tickPeriod = hpetTimer->capabilities.tickPeriod;
        LogInfo("HPET: Tick Period - %uns", tickPeriod / 1000000);

        hpetTimer->counterValue          = 0;
        hpetTimer->configuration.enabled = 1;

        LogInfo("HPET Initialized!");
    }

    void Sleep(uint64_t milliseconds)
    {
        const uint64_t ticks = hpetTimer->counterValue
                             + (milliseconds * 1000000000000) / tickPeriod;
        while (hpetTimer->counterValue < ticks) __asm__ volatile("nop");
    }

} // namespace HPET