/*
 * Created by vitriol1744 on 28.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "BootInfo.hpp"
#include "Common.hpp"

#include "Memory/PhysicalMemoryManager.hpp"

#define HIGHER_HALF_OFFSET BootInfo::GetHHDMOffset()

using PhysAddr                                              = uintptr_t;
using VirtAddr                                              = uintptr_t;

inline static constexpr const uint64_t PTE_PRESENT          = BIT(0);
inline static constexpr const uint64_t PTE_WRITABLE         = BIT(1);
inline static constexpr const uint64_t PTE_USER_ACCESSIBLE  = BIT(2);
inline static constexpr const uint64_t PTE_WRITE_THROUGH    = BIT(3);
inline static constexpr const uint64_t PTE_CACHING_DISABLED = BIT(4);
inline static constexpr const uint64_t PTE_ACCESSED         = BIT(5);
inline static constexpr const uint64_t PTE_LARGE_PAGE       = BIT(7);
inline static constexpr const uint64_t PTE_EXECUTE_DISABLE
    = 0x8000000000000000ULL;

template <typename Address>
inline static constexpr bool IsHigherHalfAddress(Address addr)
{
    return reinterpret_cast<VirtAddr>(addr) >= HIGHER_HALF_OFFSET;
}
template <typename T>
inline static constexpr T ToHigherHalfAddress(PhysAddr addr)
{
    T ret = IsHigherHalfAddress(addr)
              ? reinterpret_cast<T>(addr)
              : reinterpret_cast<T>(addr + HIGHER_HALF_OFFSET);
    return ret;
}
template <typename T>
inline static constexpr T FromHigherHalfAddress(VirtAddr addr)
{
    T ret = !IsHigherHalfAddress(addr)
              ? reinterpret_cast<T>(addr)
              : reinterpret_cast<T>(addr - HIGHER_HALF_OFFSET);
    return ret;
}

class PageMap
{
  public:
    PageMap() = default;

    inline void Create()
    {
        topLevel = reinterpret_cast<uintptr_t>(
            PhysicalMemoryManager::CallocatePages(1));
    }

    void MapPage(VirtAddr virtAddr, PhysAddr physAddr, uint64_t flags) const;

    [[nodiscard]]
    inline bool Exists() const
    {
        return topLevel != 0;
    }
    [[nodiscard]]
    inline PhysAddr GetTopLevel() const
    {
        return topLevel + HIGHER_HALF_OFFSET;
    }

  private:
    PhysAddr topLevel = 0;
};

namespace VirtualMemoryManager
{
    void     Initialize();
    void     SwitchPageMap(PageMap& page_map);

    PageMap& GetKernelPageMap();
} // namespace VirtualMemoryManager
namespace VMM = VirtualMemoryManager;
