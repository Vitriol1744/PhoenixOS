/*
 * Created by vitriol1744 on 03.07.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wkeyword-macro"
#define unsigned signed
    typedef __SIZE_TYPE__ ssize_t;
#undef unsigned
#pragma clang diagnostic pop

    typedef int64_t      blkcnt_t;
    typedef int64_t      blksize_t;
    typedef uint64_t     clock_t;
    typedef int          clockid_t;
    typedef uint64_t     dev_t;
    typedef uint64_t     fsblkcnt_t;
    typedef uint64_t     fsfilcnt_t;
    typedef uint32_t     gid_t;
    typedef int32_t      id_t;
    typedef uint64_t     ino_t;
    typedef uint64_t     key_t;
    typedef unsigned int mode_t;
    typedef uint64_t     nlink_t;
    typedef int64_t      off_t;
    typedef int32_t      pid_t;
    typedef uint32_t     uid_t;
    typedef int64_t      time_t;

#ifdef __cplusplus
}
#endif