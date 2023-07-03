/*
 * Created by vitriol1744 on 19.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "VirtualFileSystem/INode.hpp"
#include "VirtualFileSystem/TmpFs/TmpFsINode.hpp"
#include "VirtualFileSystem/VirtualFileSystem.hpp"

struct TmpFs : public Filesystem
{
    size_t maxInodes   = 0;
    size_t maxSize     = 0;

    size_t currentSize = 0;

    TmpFs();

    virtual INode* Mount(INode* parent, INode* source, INode* target,
                         std::string_view name, void* data = nullptr) override;
    virtual INode* CreateNode(INode* parent, std::string_view name, mode_t mode,
                              INodeType type) override;
    virtual INode* Symlink(INode* parent, std::string_view name,
                           std::string_view target) override;
    virtual INode* Link(INode* parent, std::string_view name,
                        INode* oldNode) override;
    virtual bool   Populate(INode* node) override { return false; }
};
