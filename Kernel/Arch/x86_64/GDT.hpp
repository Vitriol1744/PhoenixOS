/*
 * Created by vitriol1744 on 29.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Common.hpp"

#include <cstdint>

#define KERNEL_CODE_SELECTOR 0x08ull
#define KERNEL_DATA_SELECTOR 0x10ull
#define USERLAND_CODE_SELECTOR (0x18ull | 3ull)
#define USERLAND_DATA_SELECTOR (0x20ull | 3ull)

#define PH_UNUSED [[maybe_unused]]

#pragma pack(push, 1)
struct TaskStateSegment
{
    PH_UNUSED uint32_t reserved1;
    PH_UNUSED uint64_t rsp[3];
    PH_UNUSED uint64_t reserved2;
    PH_UNUSED uint64_t ist[7];
    PH_UNUSED uint64_t reserved3;
    PH_UNUSED uint16_t reserved4;
    PH_UNUSED uint16_t ioMapBase;
};
#pragma pack(pop)

namespace GDT
{
    void Initialize();
    void Load();

    void LoadTSS(TaskStateSegment* tss);
}
