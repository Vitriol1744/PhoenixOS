/*
 * Created by vitriol1744 on 13.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

struct MountPoint
{
    MountPoint() = default;
    MountPoint(class Filesystem* fs)
        : fs(fs)
        , mounted(true)
    {
    }

    bool IsMounted() { return mounted; }

  private:
    Filesystem* fs;
    bool        mounted = false;
};