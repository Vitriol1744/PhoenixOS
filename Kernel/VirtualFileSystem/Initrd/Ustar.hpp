/*
 * Created by vitriol1744 on 19.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "cstdint"

namespace Ustar
{
#pragma pack(push, 1)
    struct FileHeader
    {
        char filename[100];
        char mode[8];
        char uid[8];
        char gid[8];
        char fileSize[12];
        char mtime[12];
        char checksum[8];
        char type;
        char linkName[100];
        char signature[6];
        char version[2];
        char userName[32];
        char groupName[32];
        char deviceMajor[8];
        char deviceMinor[8];
        char filenamePrefix[155];
    } __attribute__((packed));
#pragma pack(pop)

    inline static constexpr const char* const MAGIC               = "ustar";
    inline static constexpr const uint8_t     MAGIC_LENGTH        = 6;

    // NOTE: Both '0' and '\0' mean the same file type
    inline static constexpr const uint8_t     FILE_TYPE_NORMAL    = '\0';
    inline static constexpr const uint8_t     FILE_TYPE_NORMAL_   = '0';
    inline static constexpr const uint8_t     FILE_TYPE_HARD_LINK = '1';
    inline static constexpr const uint8_t     FILE_TYPE_SYMLINK   = '2';
    inline static constexpr const uint8_t     FILE_TYPE_CHARACTER_DEVICE = '3';
    inline static constexpr const uint8_t     FILE_TYPE_BLOCK_DEVICE     = '4';
    inline static constexpr const uint8_t     FILE_TYPE_DIRECTORY        = '5';
    inline static constexpr const uint8_t     FILE_TYPE_FIFO             = '6';
    inline static constexpr const uint8_t     FILE_TYPE_CONTIGUOUS       = '7';

    bool                                      Validate(uintptr_t address);
    void                                      Load(uintptr_t address);
} // namespace Ustar