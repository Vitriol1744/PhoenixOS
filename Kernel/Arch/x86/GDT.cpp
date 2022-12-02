#include "GDT.hpp"

#include "KLibC.hpp"

inline static constexpr const uint32_t GDT_ACCESS_CODE_READABLE    = BIT(1);
inline static constexpr const uint32_t GDT_ACCESS_DATA_WRITABLE    = BIT(1);
inline static constexpr const uint32_t GDT_ACCESS_DATA_GROWS_DOWN  = BIT(2);
inline static constexpr const uint32_t GDT_ACCESS_CODE_RING_LESSEQ = BIT(2);
inline static constexpr const uint32_t GDT_ACCESS_CODE_SEGMENT     = BIT(3);
inline static constexpr const uint32_t GDT_ACCESS_CODE_OR_DATA     = BIT(4);
inline static constexpr const uint32_t GDT_ACCESS_RING1            = BIT(5);
inline static constexpr const uint32_t GDT_ACCESS_RING2            = BIT(6);
inline static constexpr const uint32_t GDT_ACCESS_RING3   = BIT(5) | BIT(6);
inline static constexpr const uint32_t GDT_ACCESS_PRESENT = BIT(7);

inline static constexpr const uint32_t GDT_FLAG_64BIT_DESCRIPTOR = BIT(1);
inline static constexpr const uint32_t GDT_FLAG_32BIT_DESCRIPTOR = BIT(2);
inline static constexpr const uint32_t GDT_FLAG_GRANULARITY_4K   = BIT(3);

#pragma pack(push, 1)
struct SegmentDescriptor
{
    uint16_t limitLow;
    uint16_t baseLow;
    uint8_t  baseMiddle;
    uint8_t  access;
    uint8_t  limitHigh : 4;
    uint8_t  flags     : 4;
    uint8_t  baseHigh;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct
{
    SegmentDescriptor null;
    SegmentDescriptor kernelCode;
    SegmentDescriptor kernelData;
    SegmentDescriptor userCode;
    SegmentDescriptor userData;
} gdtEntries;
#pragma pack(pop)

#define GDTWriteEntry(_entry, _base, _limit, _access)                          \
    {                                                                          \
        (_entry)->limitLow   = _limit & 0xffff;                                \
        (_entry)->baseLow    = _base & 0xffff;                                 \
        (_entry)->baseMiddle = (_base >> 16) & 0xff;                           \
        (_entry)->access     = _access | GDT_ACCESS_PRESENT;                   \
        (_entry)->limitHigh  = (_limit >> 16) & 0xf;                           \
        (_entry)->flags = GDT_FLAG_64BIT_DESCRIPTOR | GDT_FLAG_GRANULARITY_4K; \
        (_entry)->baseHigh = (_base >> 24) & 0xff;                             \
    }

namespace GDT
{
    void Initialize()
    {
        memset(&gdtEntries.null, 0, sizeof(SegmentDescriptor));

        uint8_t kernelCodeAccess = GDT_ACCESS_CODE_READABLE
                                 | GDT_ACCESS_CODE_SEGMENT
                                 | GDT_ACCESS_CODE_OR_DATA;
        uint8_t kernelDataAccess
            = GDT_ACCESS_DATA_WRITABLE | GDT_ACCESS_CODE_OR_DATA;
        uint8_t userCodeAccess = GDT_ACCESS_CODE_READABLE
                               | GDT_ACCESS_CODE_SEGMENT
                               | GDT_ACCESS_CODE_OR_DATA | GDT_ACCESS_RING3;
        uint8_t userDataAccess
            = GDT_ACCESS_DATA_WRITABLE | GDT_ACCESS_CODE_READABLE;
        GDTWriteEntry(&gdtEntries.kernelCode, 0, 0, kernelCodeAccess);
        GDTWriteEntry(&gdtEntries.kernelData, 0, 0, kernelDataAccess);
        GDTWriteEntry(&gdtEntries.userCode, 0, 0, userCodeAccess);
        GDTWriteEntry(&gdtEntries.userData, 0, 0, userDataAccess);
    }
    void Load()
    {
#pragma pack(push, 1)
        struct
        {
            uint16_t  limit;
            uintptr_t base;
        } gdtr;
#pragma pack(pop)
        gdtr.limit = sizeof(gdtEntries) - 1;
        gdtr.base  = reinterpret_cast<uintptr_t>(&gdtEntries);

        __asm__ volatile(
            "lgdt %1\n"
            "push %0\n"
            "lea rax, [.l1]\n"
            "push rax\n"
            "retfq\n"
            ".l1:\n"
            "mov ax, 0x10\n"
            "mov ds, ax\n"
            "mov es, ax\n"
            "mov fs, ax\n"
            "mov gs, ax\n"
            "mov ss, ax\n"
            :
            : "r"(GDT_KERNEL_CODE_SELECTOR64), "m"(gdtr));
    }
} // namespace GDT