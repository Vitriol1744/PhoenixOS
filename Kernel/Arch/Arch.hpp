/*
 * Created by vitriol1744 on 25.11.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#define PH_ARCH_X86_64 0x0001
#define PH_ARCH_IA32   0x0002

namespace Arch
{
    void Initialize();
}