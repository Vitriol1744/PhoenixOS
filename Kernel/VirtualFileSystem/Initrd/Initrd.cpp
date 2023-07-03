/*
 * Created by vitriol1744 on 19.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "Initrd.hpp"

#include "Memory/PhysicalMemoryManager.hpp"
#include "Memory/VirtualMemoryManager.hpp"
#include "Utility/Logger.hpp"
#include "Utility/Math.hpp"
#include "VirtualFileSystem/Initrd/Ustar.hpp"

namespace Initrd
{
    bool Initialize()
    {
        auto initrd = BootInfo::FindModule("initrd");
        if (!initrd)
        {
            LogError("Could not find initrd module!");
            return false;
        }

        uintptr_t address = ToHigherHalfAddress<uintptr_t>(
            reinterpret_cast<uintptr_t>(initrd->address));

        if (Ustar::Validate(address))
        {
            LogTrace("Initrd: Trying to load USTAR archive...");
            Ustar::Load(address);
        }
        else
        {
            LogError("Initrd: Unknown archive format!");
            return false;
        }

        size_t pageCount = (Math::AlignUp(initrd->size, 512) + 512) / PAGE_SIZE;
        PhysicalMemoryManager::FreePages(FromHigherHalfAddress<void*>(address),
                                         pageCount);
        return true;
    }
} // namespace Initrd