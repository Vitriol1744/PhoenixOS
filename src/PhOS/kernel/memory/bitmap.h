#ifndef BITMAP_H
#define BITMAP_H

#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint8_t* indices;
    size_t   size;
} bitmap_t;

inline static void bitmap_SetIndex(bitmap_t* bitmap, uint16_t index, bool value)
{
    uint8_t bit = 1 << index % 8;
    if (value) { bitmap->indices[index / 8] |= bit; }
    else
        bitmap->indices[index / 8] &= ~bit;
}

inline static bool bitmap_GetIndex(bitmap_t* bitmap, uint16_t index)
{
    if (index > bitmap->size) return false;

    uint64_t byte_index = index / 8;
    uint8_t  bit_value  = (uint8_t)(1 << (index % 8));

    return bitmap->indices[byte_index] & bit_value;
}

#endif // BITMAP_H