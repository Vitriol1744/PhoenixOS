/*
 * Created by vitriol1744 on 20.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include <errno.h>

static errno_t      error;

extern "C" errno_t* __errno_location()
{
    return reinterpret_cast<errno_t*>(&error);
}