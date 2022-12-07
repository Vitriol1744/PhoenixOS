#pragma once

#include "Common.hpp"

#include "Utility/KLibC.hpp"

struct Bitmap
{
    Bitmap() = default;
    Bitmap(uint8_t* data, const size_t size, uint8_t value = 1)
        : data(data)
        , size(size)
    {
        SetAll(value);
    }

    inline void SetAll(uint8_t value) { memset(data, value, size); }
    inline void SetIndex(uint64_t index, bool value)
    {
        const uint64_t byte = index / 8;
        const uint64_t bit  = index % 8;

        if (value) data[byte] |= BIT(bit);
        else data[byte] &= ~BIT(bit);
    }
    inline bool GetIndex(uint64_t index)
    {
        const uint64_t byte = index / 8;
        const uint64_t bit  = index % 8;

        return data[byte] & BIT(bit);
    }

    uint8_t* data;
    size_t   size;
};