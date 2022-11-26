#include "PhysicalMemoryManager.hpp"

#include "BootInfo.hpp"

namespace PhysicalMemoryManager
{
    static MemoryMapEntry* largestEntry = nullptr;

    bool                   Initialize()
    {
        uint64_t  entryCount       = 0;
        MemoryMap memoryMap        = BootInfo::GetMemoryMap(entryCount);
        uint64_t  largestEntrySize = 0;
        for (uint32_t i = 0; i < entryCount; i++)
        {
            LogTrace(
                "MemoryMapEntry[%02d]: Base: %#010p, Length: %#010x, Type: "
                "%d\n",
                i, memoryMap[i]->base, memoryMap[i]->length,
                memoryMap[i]->type);

            if (memoryMap[i]->type == MemoryMapEntryType::eUsable
                && memoryMap[i]->length > largestEntrySize)
                largestEntry = memoryMap[i];
        }
    }
} // namespace PhysicalMemoryManager