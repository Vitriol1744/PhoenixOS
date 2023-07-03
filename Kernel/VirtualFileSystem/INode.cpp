/*
 * Created by vitriol1744 on 20.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "INode.hpp"

INode* INode::InternalReduce(bool symlinks, bool automount, size_t cnt)
{
    if (mountGate && automount)
        return mountGate->InternalReduce(symlinks, automount, 0);

    if (!target.empty() && symlinks)
    {
        if (cnt >= SYMLOOP_MAX - 1)
        {
            errno = ELOOP;
            return nullptr;
        }

        auto nextNode = std::get<1>(
            VirtualFileSystem::ResolvePath(parent, target, automount));
        if (!nextNode)
        {
            errno = ENOENT;
            return nullptr;
        }

        return nextNode->InternalReduce(symlinks, automount, ++cnt);
    }

    return this;
}

INode* INode::Reduce(bool symlinks, bool automount)
{
    return InternalReduce(symlinks, automount, 0);
}

std::string INode::GetPath()
{
    std::string ret("");

    auto        current = this;
    auto        root    = VirtualFileSystem::GetRootINode();

    while (current && current != root)
    {
        ret.insert(0, "/" + current->name);
        current = current->parent;
    }

    if (ret.empty()) ret += "/";
    return ret;
}

mode_t INode::GetMode() const { return stats.st_mode & ~S_IFMT; }

bool   INode::IsEmpty()
{
    filesystem->Populate(this);
    return children.empty();
}