/*
 * Created by vitriol1744 on 11.12.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Common.hpp"

#include "VFS/Filesystem.hpp"

namespace VirtualFileSystem
{
    struct MountPoint
    {
        Filesystem* fs;
    };

    void Initialize()
    {
    }
}