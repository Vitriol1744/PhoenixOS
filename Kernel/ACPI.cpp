#include "ACPI.hpp"

#include "BootInfo.hpp"
#include "Common.hpp"

#include "Utility/KLibC.hpp"

namespace ACPI
{
    struct RSDP
    {
        char     signature[8];
        uint8_t  checksum;
        char     oemID[6];
        uint8_t  revision;
        uint32_t rsdtAddress;
        uint32_t length;
        uint64_t xsdtAddress;
        uint8_t  extendedChecksum;
        uint8_t  reserved[3];
    } __attribute__((packed));
    struct RSDT
    {
        SDTHeader header;
        char      sdts[];
    } __attribute__((packed));

    static bool  xsdt = false;
    static RSDT* rsdt;

    static bool  ValidateChecksum(SDTHeader* header)
    {
        uint32_t checksum = 0;
        for (int i = 0; i < header->length; i++) checksum += ((char*)header)[i];

        return checksum;
    }
    static uintptr_t GetTablePointer(uint8_t index)
    {
        if (xsdt)
        {
            const uint64_t* ptr = (uint64_t*)rsdt->sdts;

            return ptr[index] + BootInfo::GetHHDMOffset();
        }

        const uint32_t* ptr = (uint32_t*)rsdt->sdts;
        return (uint64_t)ptr[index] + (uint64_t)BootInfo::GetHHDMOffset();
    }

    // TODO: FADT and ACPI mode
    void Initialize()
    {
        RSDP* rsdp = reinterpret_cast<RSDP*>(BootInfo::GetRSDPAddress());
        xsdt       = rsdp->revision == 2 && rsdp->xsdtAddress != 0;

        uint64_t rsdtPointer = xsdt ? rsdp->xsdtAddress : rsdp->rsdtAddress;
        rsdt = reinterpret_cast<RSDT*>(rsdtPointer + BootInfo::GetHHDMOffset());
        Assert(rsdt != nullptr);
    }
    SDTHeader* GetTable(const char* signature, size_t index)
    {
        Assert(signature != nullptr);

        size_t       count      = 0;
        const size_t entryCount = (rsdt->header.length - sizeof(SDTHeader)) / 4;
        for (size_t i = 0; i < entryCount; ++i)
        {
            SDTHeader* header
                = reinterpret_cast<SDTHeader*>(GetTablePointer(i));

            Assert(header != nullptr && header->signature != nullptr);
            Assert(ValidateChecksum(header) != false);

            if (!strncmp(header->signature, signature, 4)) return header;
        }

        LogError("Failed to find %s", signature);
        return nullptr;
    }
} // namespace ACPI