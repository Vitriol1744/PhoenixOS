/*
 * Created by vitriol1744 on 19.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "VirtualFileSystem/Filesystem.hpp"
#include "Utility/Path.hpp"

#include <cerrno>
#include <cstdio>
#include <memory>
#include <stddef.h>
#include <tuple>
#include <unordered_map>
#include <vector>

class INode;
namespace VirtualFileSystem
{
    INode* GetRootINode();

    void   RecursiveDelete(INode* node);

    std::tuple<INode*, INode*, std::string>
    ResolvePath(INode* parent, std::string_view path, bool automount = true);

    bool   MountRoot(std::string_view filesystemName);
    bool   Mount(INode* parent, PathView source, PathView target,
                 std::string_view fsName, int flags = 0, void* data = nullptr);
    bool   Unmount(INode* parent, PathView path, int flags = 0);

    INode* CreateNode(INode* parent, PathView path, mode_t mode,
                      INodeType type);
    INode* Symlink(INode* parent, PathView path, std::string_view target);

    INode* Link(INode* oldParent, PathView oldPath, INode* newParent,
                PathView newPath, int flags = 0);
    bool   Unlink(INode* parent, PathView path, int flags = 0);
} // namespace VirtualFileSystem

namespace VFS = VirtualFileSystem;
