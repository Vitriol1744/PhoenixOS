/*
 * Created by vitriol1744 on 21.03.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Utility/Path.hpp"
#include "Utility/UnixTypes.hpp"

#include <atomic>
#include <cerrno>
#include <cstdlib>
#include <mutex>
#include <optional>
#include <string>

class INode;
enum class INodeType;
class Filesystem
{
  public:
    Filesystem(const std::string& name)
        : mountedOn(nullptr)
        , mountData(nullptr)
        , name(name)
    {
    }

    inline INode*  GetMountedOn() { return mountedOn; }
    inline INode*  GetRootINode() { return root; }
    inline ino_t   GetNextINodeIndex() { return nextInodeIndex++; }
    inline dev_t   GetDeviceID() const { return deviceID; }

    virtual INode* Mount(INode* parent, INode* source, INode* target,
                         std::string_view name, void* data = nullptr)
        = 0;
    virtual INode* CreateNode(INode* parent, std::string_view name, mode_t mode,
                              INodeType type)
        = 0;
    virtual INode* Symlink(INode* parent, std::string_view name,
                           std::string_view target)
        = 0;

    virtual INode* Link(INode* parent, std::string_view name, INode* old_node)
        = 0;
    virtual bool Populate(INode* node) = 0;

  protected:
    INode*             mountedOn = nullptr;
    void*              mountData = nullptr;
    INode*             root      = nullptr;

    std::mutex         lock;

    std::string        name;
    std::atomic<ino_t> nextInodeIndex = 0;
    dev_t              deviceID       = 0;
};
