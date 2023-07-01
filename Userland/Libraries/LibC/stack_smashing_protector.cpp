/*
 * Created by vitriol1744 on 13.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "Common.hpp"

#include <cassert>

#if UINT32_MAX == UINTPTR_MAX
    #define STACK_CHK_GUARD 0xe2dee396
#else
    #define STACK_CHK_GUARD 0x595e9fbd94fda766
#endif
extern "C" uintptr_t                      __stack_chk_guard = STACK_CHK_GUARD;
extern "C" __attribute__((noreturn)) void __stack_chk_fail() { assert(false); }