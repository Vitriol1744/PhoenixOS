/*
 * Created by vitriol1744 on 03.07.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "API/POSIX/sys/time.h"

#define S_IFMT      0170000
#define S_IFBLK     0060000
#define S_IFCHR     0020000
#define S_IFIFO     0010000
#define S_IFREG     0100000
#define S_IFDIR     0040000
#define S_IFLNK     0120000
#define S_IFSOCK    0140000

#define S_ISBLK(m)  (((m)&S_IFMT) == S_IFBLK)
#define S_ISCHR(m)  (((m)&S_IFMT) == S_IFCHR)
#define S_ISDIR(m)  (((m)&S_IFMT) == S_IFDIR)
#define S_ISFIFO(m) (((m)&S_IFMT) == S_IFIFO)
#define S_ISREG(m)  (((m)&S_IFMT) == S_IFREG)
#define S_ISLNK(m)  (((m)&S_IFMT) == S_IFLNK)
#define S_ISSOCK(m) (((m)&S_IFMT) == S_IFSOCK)

#ifdef __cplusplus
extern "C"
{
#endif

    struct stat
    {
        dev_t           st_dev;
        ino_t           st_ino;
        mode_t          st_mode;
        nlink_t         st_nlink;
        uid_t           st_uid;
        gid_t           st_gid;
        dev_t           st_rdev;
        off_t           st_size;

        blksize_t       st_blksize;
        blkcnt_t        st_blocks;

        struct timespec st_atim;
        struct timespec st_mtim;
        struct timespec st_ctim;
    };

#ifdef __cplusplus
}
#endif