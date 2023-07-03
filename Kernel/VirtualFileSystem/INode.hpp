/*
 * Created by vitriol1744 on 21.03.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "VirtualFileSystem/VirtualFileSystem.hpp"

#include <cstdint>
#include <details/string_view>
#include <mutex>

enum class INodeType
{
    eRegular         = 0,
    eHardLink        = 1,
    eSymlink         = 2,
    eCharacterDevice = 3,
    eBlockDevice     = 4,
    eDirectory       = 5,
};

class INode
{
  public:
    std::string target;
    INode*      mountGate;

    INode(INode* parent, std::string_view name, Filesystem* fs)
        : parent(parent)
        , name(name)
        , filesystem(fs)
    {
    }
    INode(std::string_view name, INodeType type)
        : name(name)
        , type(type)
    {
    }
    INode(INode* parent, std::string_view name, Filesystem* fs, INodeType type)
        : parent(parent)
        , name(name)
        , filesystem(fs)
        , type(type)
    {
    }

    virtual ~INode() {}

    INode*             Reduce(bool symlinks, bool automount = true);
    std::string        GetPath();

    inline Filesystem* GetFilesystem() { return filesystem; }
    inline const stat& GetStats() const { return stats; }
    inline std::unordered_map<std::string_view, INode*>& GetChildren()
    {
        return children;
    }
    inline const std::string& GetName() { return name; }
    inline INode*             GetParent() { return parent; }
    mode_t                    GetMode() const;
    INodeType                 GetType() const { return type; }

    bool                      IsEmpty();

    virtual void    InsertChild(INode* node, std::string_view name)       = 0;
    virtual ssize_t Read(void* buffer, off_t offset, size_t bytes)        = 0;
    virtual ssize_t Write(const void* buffer, off_t offset, size_t bytes) = 0;

  protected:
    std::mutex                                   lock;
    Filesystem*                                  filesystem;
    stat                                         stats;
    std::unordered_map<std::string_view, INode*> children;

    std::string                                  name;

    INode*                                       parent;

    INodeType                                    type;

    INode* InternalReduce(bool symlinks, bool automount, size_t cnt);
};