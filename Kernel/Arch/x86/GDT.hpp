/*
 * Created by vitriol1744 on 25.11.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include <stdint.h>

#define KERNEL_CODE_SELECTOR 0x08ull
#define KERNEL_DATA_SELECTOR 0x10ull
#define USERLAND_CODE_SELECTOR (0x18ull | 3ull)
#define USERLAND_DATA_SELECTOR (0x20ull | 3ull)

#pragma pack(push, 1)
struct TaskStateSegment
{
    uint32_t reserved1;
    uint64_t rsp[3];
    uint64_t reserved2;
    uint64_t ist[7];
    uint64_t reserved3;
    uint16_t reserved4;
    uint16_t ioMapBase;
};
#pragma pack(pop)

namespace GDT
{
    void Initialize();
    void Load();

    void LoadTSS(TaskStateSegment* tss);
} // namespace GDT
