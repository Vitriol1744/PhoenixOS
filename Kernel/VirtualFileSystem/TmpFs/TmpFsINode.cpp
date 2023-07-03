/*
 * Created by vitriol1744 on 28.03.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "TmpFsINode.hpp"

#include "Memory/KernelHeap.hpp"
#include "Utility/Math.hpp"
#include "VirtualFileSystem/TmpFs/TmpFs.hpp"

ssize_t TmpFsINode::Read(void* buffer, off_t offset, size_t bytes)
{
    std::unique_lock guard(lock);

    size_t           count = bytes;
    if (ssize_t(offset + bytes) >= stats.st_size)
        count = bytes - ((offset + bytes) - stats.st_size);

    memcpy(buffer, reinterpret_cast<uint8_t*>(data) + offset, count);
    return count;
}
ssize_t TmpFsINode::Write(const void* buffer, off_t offset, size_t bytes)
{
    std::unique_lock guard(lock);

    if (offset + bytes > capacity)
    {
        size_t newCapacity = capacity;
        while (offset + bytes >= newCapacity) newCapacity *= 2;

        auto tfs = reinterpret_cast<TmpFs*>(filesystem);
        if (tfs->currentSize + (newCapacity - capacity) > tfs->maxSize)
        {
            errno = ENOSPC;
            return -1;
        }

        data = static_cast<uint8_t*>(KernelHeap::Reallocate(data, newCapacity));
        if (!data) return -1;

        capacity = newCapacity;
    }

    memcpy(data + offset, buffer, bytes);

    if (off_t(offset + bytes) >= stats.st_size)
    {
        stats.st_size   = off_t(offset + bytes);
        stats.st_blocks = Math::DivRoundUp(stats.st_size, stats.st_blksize);
    }

    return bytes;
}
