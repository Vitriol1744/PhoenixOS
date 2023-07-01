/*
 * Created by vitriol1744 on 28.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

#define BIT(n) (1 << n)

struct Bitmap
{
    Bitmap() = default;
    Bitmap(uint8_t* data, const size_t size, uint8_t value = 1)
        : data(data)
        , size(size)
    {
        SetAll(value);
    }

    inline void SetAll(const uint8_t value) { memset(data, value, size); }
    inline void SetIndex(const uint64_t index, const bool value)
    {
        const uint64_t byte = index / 8;
        const uint64_t bit  = index % 8;

        if (value) data[byte] |= BIT(bit);
        else data[byte] &= ~BIT(bit);
    }
    inline bool GetIndex(const uint64_t index) const
    {
        const uint64_t byte = index / 8;
        const uint64_t bit  = index % 8;

        return data[byte] & BIT(bit);
    }

    uint8_t* data;
    size_t size;
};