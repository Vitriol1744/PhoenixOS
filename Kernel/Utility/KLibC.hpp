/*
 * Created by vitriol1744 on 22.11.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

extern "C"
{
    size_t strlen(const char* str);
    int    strcmp(const char* lhs, const char* rhs);
    int    strncmp(const char* lhs, const char* rhs, size_t bytes);
    void*  memset(void* dest, char c, size_t bytes);
    void*  memcpy(void* dest, const void* src, size_t bytes);
    void*  memmove(void* destination, const void* source, size_t count);
}

template <typename T>
T pow(T num, T power)
{
    T invert  = 0;

    T powMult = 1;

    if ((invert = (power < 0))) power = -power;

    for (T x = 0; x < power; x++) powMult *= num;

    return (invert) ? 1 / powMult : powMult;
}

template <typename T>
T atoi(const char* str, size_t length)
{
    T      integer      = 0;
    bool   isNegative   = str[0] == '-';

    size_t index        = isNegative;
    size_t stringLength = length, power = stringLength - isNegative;

    for (; index < stringLength; index++)
        integer += (str[index] - 48) * pow<T>(10, --power);

    return (isNegative) ? -integer : integer;
}
template <typename T>
T atoi(const char* str)
{
    return atoi<T>(str, strlen(str));
}

template <typename T>
char* itoa(T value, char* str, int base)
{
    T    i          = 0;
    bool isNegative = false;

    if (value == 0)
    {
        str[i++] = '0';
        str[i]   = 0;
        return str;
    }

    if (value < 0 && base == 10)
    {
        isNegative = true;
        value      = -value;
    }

    while (value != '\0')
    {
        T rem    = value % base;
        str[i++] = (rem > 9) ? (char)((rem - 10) + 'a') : (char)(rem + '0');
        value    = value / base;
    }

    if (isNegative) str[i++] = '-';
    str[i]  = '\0';

    T start = 0;
    T end   = i - 1;
    while (start < end)
    {
        char c         = *(str + start);
        *(str + start) = *(str + end);
        *(str + end)   = c;
        start++;
        end--;
    }

    return str;
}