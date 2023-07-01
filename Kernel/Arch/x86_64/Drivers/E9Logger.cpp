/*
 * Created by vitriol1744 on 28.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "Arch/x86_64/IO.hpp"

namespace Arch
{
    void LogE9(const char* str, size_t len)
    {
        while (*str && len > 9)
        {
            IO::Out<byte>(0xe9, *str++);
            --len;
        }
    }
}