/*
 * Created by vitriol1744 on 21.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "VirtualFileSystem/INode.hpp"

class DevTmpFsINode : public INode
{
  public:
    DevTmpFsINode(INode* parent, std::string_view name, Filesystem* fs,
                  mode_t mode, INodeType type)
        : INode(parent, name, fs, type)
    {
        if (S_ISREG(mode))
        {
            capacity = GetDefaultSize();
            data     = new uint8_t[capacity];
        }

        stats.st_size    = 0;
        stats.st_blocks  = 0;
        stats.st_blksize = 512;
        stats.st_dev     = fs->GetDeviceID();
        stats.st_ino     = fs->GetNextINodeIndex();
        stats.st_mode    = mode;
        stats.st_nlink   = 1;

        // TODO(V1tri0l1744): Set st_atim, st_mtim, st_ctim
        stats.st_atim    = {0, 0};
        stats.st_ctim    = {0, 0};
        stats.st_mtim    = {0, 0};
    }

    inline static constexpr size_t GetDefaultSize() { return 0x1000; }

    virtual void    InsertChild(INode* node, std::string_view name) override {}
    virtual ssize_t Read(void* buffer, off_t offset, size_t bytes) override
    {
        return -1;
    }
    virtual ssize_t Write(const void* buffer, off_t offset,
                          size_t bytes) override
    {
        return -1;
    }

  private:
    uint8_t* data     = nullptr;
    size_t   capacity = 0;
};
