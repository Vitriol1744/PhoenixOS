/*
 * Created by vitriol1744 on 19.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "TmpFs.hpp"
#include "Memory/PhysicalMemoryManager.hpp"
#include "TmpFsINode.hpp"

TmpFs::TmpFs()
    : Filesystem("TmpFs")
    , maxInodes(0)
    , maxSize(0)
    , currentSize(0)
{
}

INode* TmpFs::Mount(INode* parent, INode* source, INode* target,
                    std::string_view name, void* data)
{
    mountData
        = data ? reinterpret_cast<void*>(strdup(static_cast<const char*>(data)))
               : nullptr;

    maxSize   = PhysicalMemoryManager::GetTotalMemory() / 2;
    maxInodes = PhysicalMemoryManager::GetTotalMemory() / PAGE_SIZE / 2;

    root      = CreateNode(parent, name, 0644 | S_IFDIR, INodeType::eDirectory);
    if (root) mountedOn = target;
    return root;
}
INode* TmpFs::CreateNode(INode* parent, std::string_view name, mode_t mode,
                         INodeType type)
{
    if (nextInodeIndex >= maxInodes
        || (S_ISREG(mode)
            && currentSize + TmpFsINode::GetDefaultSize() > maxSize))
    {
        errno = ENOSPC;

        return nullptr;
    }

    return new TmpFsINode(parent, name, this, mode, type);
}

INode* TmpFs::Symlink(INode* parent, std::string_view name,
                      std::string_view target)
{
    if (nextInodeIndex >= maxInodes)
    {
        errno = ENOSPC;
        return nullptr;
    }

    auto node    = new TmpFsINode(parent, name, this, 0777 | S_IFLNK,
                                  INodeType::eSymlink);
    node->target = target;
    return node;
}

INode* TmpFs::Link(INode* parent, std::string_view name, INode* oldNode)
{
    if (oldNode->GetType() == INodeType::eDirectory)
    {
        errno = EISDIR;
        return nullptr;
    }

    return new TmpFsINode(parent, name, this,
                          (oldNode->GetStats().st_mode & ~S_IFMT),
                          oldNode->GetType());
}