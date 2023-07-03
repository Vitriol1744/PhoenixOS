/*
 * Created by vitriol1744 on 28.03.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Utility/Logger.hpp"
#include "VirtualFileSystem/INode.hpp"

#include <unordered_map>

class TmpFsINode final : public INode
{
  public:
    TmpFsINode(std::string_view name, INodeType type)
        : INode(name, type)
    {
        capacity = 128;
        data     = new uint8_t[capacity];
    }
    TmpFsINode(INode* parent, std::string_view name, Filesystem* fs,
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
        stats.st_mtim    = {0, 0};
        stats.st_ctim    = {0, 0};
    }

    ~TmpFsINode()
    {
        if (capacity > 0) delete data;
    }

    inline static constexpr size_t GetDefaultSize() { return 0x1000; }

    virtual void InsertChild(INode* node, std::string_view name) override
    {
        std::unique_lock guard(lock);
        children[name] = node;
    }
    virtual ssize_t Read(void* buffer, off_t offset, size_t bytes) override;
    virtual ssize_t Write(const void* buffer, off_t offset,
                          size_t bytes) override;

  private:
    uint8_t* data     = nullptr;
    size_t   size     = 0;
    size_t   capacity = 0;
};