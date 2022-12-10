/*
 * Created by vitriol1744 on 07.12.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include <stdint.h>

namespace HPET
{
    void Initialize();
    void Sleep(uint64_t milliseconds);
} // namespace HPET