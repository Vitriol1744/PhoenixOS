/*
 * Created by vitriol1744 on 07.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include <stdlib.h>

#include "Common.hpp"
#include "Memory/KernelHeap.hpp"

#include <ctype.h>

template <typename T, typename U = std::make_unsigned_t<T>>
constexpr inline T str2int(const char* nptr, char** endptr, int _base)
{
    auto base = static_cast<T>(_base);
    auto str  = nptr;

    if (base < 0 || base == 1)
    {
        if (endptr) *endptr = const_cast<char*>(nptr);
        return 0;
    }

    while (isspace(*str)) str++;

    bool negative = false;
    if (*str == '-')
    {
        negative = true;
        str++;
    }
    else if (*str == '+') str++;

    bool octal = (str[0] == '0');
    bool hex   = octal && (str[1] == 'x' || str[1] == 'X');

    if ((base == 0 || base == 16) && hex && isxdigit(str[2]))
    {
        str += 2;
        base = 16;
    }
    else if ((base == 0 || base == 8) && octal) base = 8;
    else if (base == 0) base = 10;

    U cutoff = 0;
    U cutlim = 0;
    if constexpr (std::is_unsigned_v<T>)
    {
        cutoff = std::numeric_limits<T>::max() / base;
        cutoff = std::numeric_limits<T>::max() % base;
    }
    else
    {
        T co   = negative ? std::numeric_limits<T>::min()
                          : std::numeric_limits<T>::max();
        cutlim = negative ? -(co % base) : co % base;
        co /= negative ? -base : base;
        cutoff = co;
    }

    U    total        = 0;
    bool converted    = false;
    bool out_of_range = false;

    for (char c = *str; c != '\0'; c = *++str)
    {
        U digit = 0;
        if (isdigit(c)) digit = c - '0';
        else if (isupper(c)) digit = c - 'A' + 0x10;
        else if (islower(c)) digit = c - 'a' + 0x10;
        else break;

        if (digit >= static_cast<U>(base)) break;

        if (out_of_range) continue;

        if (total >= cutoff || (total == cutoff && digit > cutlim))
            out_of_range = true;
        else
        {
            total     = (total * base) + digit;
            converted = true;
        }
    }

    if (endptr) *endptr = const_cast<char*>(converted ? str : nptr);

    if (out_of_range)
    {
        errno = ERANGE;
        if constexpr (std::is_unsigned_v<T>)
            return std::numeric_limits<T>::max();
        else
            return negative ? std::numeric_limits<T>::min()
                            : std::numeric_limits<T>::max();
    }

    return negative ? -total : total;
}

extern "C"
{
    void* calloc(size_t num, size_t size)
    {
        return KernelHeap::Callocate(num * size);
    }

    void  free(void* ptr) { return KernelHeap::Free(ptr); }

    void* malloc(size_t size) { return KernelHeap::Allocate(size); }

    void* realloc(void* oldptr, size_t size)
    {
        return KernelHeap::Reallocate(oldptr, size);
    }

    long long strtoll(const char* str, char** str_end, int base)
    {
        return str2int<long long>(str, str_end, base);
    }

    void abort() { Panic("abort"); }
} // extern "C"
