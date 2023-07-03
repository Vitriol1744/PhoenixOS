/*
 * Created by vitriol1744 on 03.07.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "API/POSIX/sys/types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    struct timespec
    {
        time_t tv_sec;
        long   tv_nsec;
    };

#ifdef __cplusplus
}
#endif