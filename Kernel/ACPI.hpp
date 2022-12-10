/*
 * Created by vitriol1744 on 07.12.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Common.hpp"

struct SDTHeader
{
    char     signature[4];
    uint32_t length;
    uint8_t  revision;
    uint8_t  checksum;
    char     oemID[6];
    char     oemTableID[8];
    uint32_t oemRevision;
    uint32_t createrID;
    uint32_t creatorRevision;
} __attribute__((packed));

namespace ACPI
{
    void       Initialize();
    SDTHeader* GetTable(const char* signature, size_t index = 0);
} // namespace ACPI