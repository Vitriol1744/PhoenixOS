/*
 * Created by vitriol1744 on 29.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "GDT.hpp"

#define BIT(n) (1 << n)

inline static constexpr const uint32_t GDT_ACCESS_CODE_READABLE    = BIT(1);
inline static constexpr const uint32_t GDT_ACCESS_DATA_WRITABLE    = BIT(1);
inline static constexpr const uint32_t GDT_ACCESS_DATA_GROWS_DOWN  = BIT(2);
inline static constexpr const uint32_t GDT_ACCESS_CODE_RING_LESSEQ = BIT(2);
inline static constexpr const uint32_t GDT_ACCESS_CODE_SEGMENT     = BIT(3);
inline static constexpr const uint32_t GDT_ACCESS_CODE_OR_DATA     = BIT(4);
inline static constexpr const uint32_t GDT_ACCESS_RING1            = BIT(5);
inline static constexpr const uint32_t GDT_ACCESS_RING2            = BIT(6);
inline static constexpr const uint32_t GDT_ACCESS_RING3            = BIT(5) | BIT(6);
inline static constexpr const uint32_t GDT_ACCESS_PRESENT          = BIT(7);

inline static constexpr const uint32_t GDT_FLAG_64BIT_DESCRIPTOR = BIT(1);
inline static constexpr const uint32_t GDT_FLAG_32BIT_DESCRIPTOR = BIT(2);
inline static constexpr const uint32_t GDT_FLAG_GRANULARITY_4K   = BIT(3);

inline static constexpr const uint32_t TSS_FLAG_PRESENT          = BIT(3);
inline static constexpr const uint32_t TSS_FLAG_INACTIVE = BIT(3) | BIT(0);

inline static constexpr const uint32_t TSS_SELECTOR      = 0x28;

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
struct TaskStateSegmentDescriptor
{
    uint16_t length;
    uint16_t baseLow;
    uint8_t  baseMiddle1;
    uint8_t  flags1;
    uint8_t  flags2;
    uint8_t  baseMiddle2;
    uint32_t baseHigh;
    uint32_t reserved;
};
struct GDTEntries
{
    SegmentDescriptor          null;
    SegmentDescriptor          kernelCode;
    SegmentDescriptor          kernelData;
    SegmentDescriptor          userCode;
    SegmentDescriptor          userData;
    TaskStateSegmentDescriptor tss;
} gdtEntries;
#pragma pack(pop)

#define GDTWriteEntry(_entry, _base, _limit, _access, _flags)                  \
    {                                                                          \
        (_entry)->limitLow   = _limit & 0xffff;                                \
        (_entry)->baseLow    = _base & 0xffff;                                 \
        (_entry)->baseMiddle = (_base >> 16) & 0xff;                           \
        (_entry)->access     = _access | GDT_ACCESS_PRESENT;                   \
        (_entry)->limitHigh  = (_limit >> 16) & 0xf;                           \
        (_entry)->flags      = _flags;                                         \
        (_entry)->baseHigh   = (_base >> 24) & 0xff;                           \
    }
#define TSSWriteEntry(_entry, _base)                                           \
    {                                                                          \
        (_entry)->length      = sizeof(TaskStateSegment);                      \
        (_entry)->baseLow     = (uint16_t)(_base & 0xffff);                    \
        (_entry)->baseMiddle1 = (uint8_t)((_base >> 16) & 0xff);               \
        (_entry)->flags1      = ((1 << 3) << 4) | (1 << 3 | 1 << 0);           \
        (_entry)->flags2      = 0;                                             \
        (_entry)->baseMiddle2 = (uint8_t)((_base >> 24) & 0xff);               \
        (_entry)->baseHigh    = (uint32_t)(_base >> 32);                       \
        (_entry)->reserved    = 0;                                             \
    }

namespace GDT
{
    void            Initialize()
    {
        memset(&gdtEntries.null, 0, sizeof(SegmentDescriptor));

        uint8_t userCodeAccess = GDT_ACCESS_CODE_READABLE
                                 | GDT_ACCESS_CODE_SEGMENT
                                 | GDT_ACCESS_CODE_OR_DATA | GDT_ACCESS_RING3;
        uint8_t userDataAccess = GDT_ACCESS_DATA_WRITABLE
                                 | GDT_ACCESS_CODE_OR_DATA | GDT_ACCESS_RING3;

        GDTWriteEntry(&gdtEntries.kernelCode, 0, 0xffffffff,
                      GDT_ACCESS_CODE_OR_DATA | GDT_ACCESS_CODE_SEGMENT
                                                | GDT_ACCESS_CODE_READABLE,
                      0xa);

        GDTWriteEntry(&gdtEntries.kernelData, 0, 0xffffffff,
                      GDT_ACCESS_CODE_OR_DATA | GDT_ACCESS_DATA_WRITABLE, 0xa);
        GDTWriteEntry(&gdtEntries.userCode, 0, 0, userCodeAccess, 0xa);
        GDTWriteEntry(&gdtEntries.userData, 0, 0, userDataAccess, 0xa);
        TSSWriteEntry(&gdtEntries.tss, 0);

        LogInfo("GDT Initialized!");
    }

    void Load() {
#pragma pack(push, 1)
        struct {
            uint16_t limit;
            uintptr_t base;
        } gdtr;
#pragma pack(pop)
        gdtr.limit = sizeof(gdtEntries) - 1;
        gdtr.base = reinterpret_cast<uintptr_t>(&gdtEntries);

        __asm__ volatile(
                "lgdt %0\n"
                "mov %%rsp, %%rbx\n"
                "push %1\n"
                "push %%rbx\n"
                "pushf\n"
                "push %2\n"
                "push $1f\n"
                "iretq\n"
                "1:\n"
                "mov %1, %%ds\n"
                "mov %1, %%es\n"
                "mov %1, %%fs\n"
                "mov %1, %%gs\n"
                "mov %1, %%ss"
                :
                : "m"(gdtr), "r"(KERNEL_DATA_SELECTOR), "r"(KERNEL_CODE_SELECTOR)
        : "rbx", "memory");
    }
}