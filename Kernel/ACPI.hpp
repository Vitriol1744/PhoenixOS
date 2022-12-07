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