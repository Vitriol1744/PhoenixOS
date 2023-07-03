/*
 * Created by vitriol1744 on 28.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include <cstddef>

namespace Arch
{
    void Initialize();
    void LogE9(const char* str, size_t len);
} // namespace Arch