#pragma once

#include "BootInfo.hpp"

#include "Memory/PhysicalMemoryManager.hpp"

using PhysAddr                                              = uintptr_t;
using VirtAddr                                              = uintptr_t;

inline static constexpr const uint32_t PTE_PRESENT          = BIT(0);
inline static constexpr const uint32_t PTE_WRITABLE         = BIT(1);
inline static constexpr const uint32_t PTE_USER_ACCESSIBLE  = BIT(2);
inline static constexpr const uint32_t PTE_WRITE_THROUGH    = BIT(3);
inline static constexpr const uint32_t PTE_CACHING_DISABLED = BIT(4);
inline static constexpr const uint32_t PTE_ACCESSED         = BIT(5);
inline static constexpr const uint32_t PTE_LARGE_PAGE       = BIT(7);
inline static constexpr const uint32_t PTE_EXECUTE_DISABLE  = BIT(63);

class PageMap
{
  public:
    PageMap() = default;
    explicit PageMap(uintptr_t topLevel)
        : topLevel(topLevel)
    {
    }

    inline void Create()
    {
        topLevel = reinterpret_cast<uintptr_t>(
            PhysicalMemoryManager::CallocatePages(1));
    }

    void        MapPage(PhysAddr physAddr, VirtAddr virtAddr, uint64_t flags);

    inline bool Exists() { return topLevel != 0; }
    inline PhysAddr GetTopLevel() { return topLevel; }

  private:
    PhysAddr topLevel = 0;
};

namespace VirtualMemoryManager
{
    void     Initialize();
    void     SwitchPageMap(PageMap& page_map);

    PageMap& GetKernelPageMap();
} // namespace VirtualMemoryManager