/*
 * Created by vitriol1744 on 19.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "DevTmpFs.hpp"

#include "VirtualFileSystem/DevTmpFs/DevTmpFsINode.hpp"

bool   DevTmpFs::Populate(INode* node) { return false; }

INode* DevTmpFs::CreateNode(INode* parent, std::string_view name, mode_t mode,
                            INodeType type)
{
    return new DevTmpFsINode(parent, name, this, mode, type);
}
INode* DevTmpFs::Symlink(INode* parent, std::string_view name,
                         std::string_view target)
{
    return nullptr;
}

INode* DevTmpFs::Link(INode* parent, std::string_view name, INode* oldNode)
{
    return nullptr;
}