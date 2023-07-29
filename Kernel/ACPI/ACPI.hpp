/*
 * Created by vitriol1744 on 17.07.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Common.hpp"

#pragma pack(push, 1)
struct SDTHeader
{
    char signature[4];
    u32  length;
    u8   revision;
    u8   checksum;
    char oemID[6];
    char oemTableID[8];
    u32  oemRevision;
    u32  creatorID;
    u32  creatorRevision;
};
#pragma pack(pop)

namespace ACPI
{
    void       Initialize();
    SDTHeader* GetTable(const char* signature, usize index = 0);
} // namespace ACPI