/*
 * Created by vitriol1744 on 07.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "string.h"

#include <cstdint>
#include <cstdlib>

extern "C"
{
    void* memcpy(void* dest, const void* src, size_t len)
    {
        uint8_t*       d = static_cast<uint8_t*>(dest);
        const uint8_t* s = static_cast<const uint8_t*>(src);

        for (size_t i = 0; i < len; i++) d[i] = s[i];

        return dest;
    }

    void* memset(void* dest, int ch, size_t len)
    {
        uint8_t* d = static_cast<uint8_t*>(dest);

        for (size_t i = 0; i < len; i++) d[i] = static_cast<uint8_t>(ch);

        return dest;
    }

    void* memmove(void* dest, const void* src, size_t len)
    {
        uint8_t*       d = static_cast<uint8_t*>(dest);
        const uint8_t* s = static_cast<const uint8_t*>(src);

        if (src > dest)
            for (size_t i = 0; i < len; i++) d[i] = s[i];
        else if (src < dest)
            for (size_t i = len; i > 0; i--) d[i - 1] = s[i - 1];

        return dest;
    }

    int memcmp(const void* ptr1, const void* ptr2, size_t len)
    {
        const uint8_t* p1 = static_cast<const uint8_t*>(ptr1);
        const uint8_t* p2 = static_cast<const uint8_t*>(ptr2);

        for (size_t i = 0; i < len; i++)
            if (p1[i] != p2[i]) return p1[i] < p2[i] ? -1 : 1;

        return 0;
    }

    void* memchr(const void* ptr, int ch, size_t len)
    {
        const uint8_t* src = static_cast<const uint8_t*>(ptr);

        while (len-- > 0)
        {
            if (*src == ch) return const_cast<uint8_t*>(src);
            src++;
        }

        return nullptr;
    }

    size_t strlen(const char* str)
    {
        size_t length = 0;
        while (str[length]) length++;
        return length;
    }

    char* strdup(const char* str)
    {
        size_t len        = strlen(str) + 1;

        void*  new_string = malloc(len);
        if (!new_string) return nullptr;

        return static_cast<char*>(memcpy(new_string, str, len));
    }

    char* strcat(char* dest, const char* src)
    {
        char* ptr = dest + strlen(dest);
        while (*src != '\0') *ptr++ = *src++;

        *ptr = '\0';
        return dest;
    }

    char* strncat(char* dest, const char* src, size_t len)
    {
        char* ptr = dest + strlen(dest);
        while (*src != '\0' && len-- > 0) *ptr++ = *src++;

        *ptr = '\0';
        return dest;
    }

    char* strchr(const char* str, int ch)
    {
        while (*str && *str != ch) str++;
        return const_cast<char*>(ch == *str ? str : nullptr);
    }

    int strcmp(const char* str1, const char* str2)
    {
        while (*str1 && *str2 && *str1 == *str2)
        {
            str1++;
            str2++;
        }
        return *str1 - *str2;
    }

    int strncmp(const char* str1, const char* str2, size_t len)
    {
        while (*str1 && *str2 && *str1 == *str2 && len-- > 0)
        {
            str1++;
            str2++;
        }

        return len != 0 ? *str1 - *str2 : 0;
    }

    char* strcpy(char* dest, const char* src)
    {
        char* ptr = dest;
        while (*src != '\0')
        {
            *dest = *src;
            dest++;
            src++;
        }

        *dest = '\0';
        return ptr;
    }

    char* strncpy(char* dest, const char* src, size_t len)
    {
        char* ret = dest;
        while (*src != '\0' && len-- > 0)
        {
            *dest = *src;
            dest++;
            src++;
        }

        *dest = '\0';
        return ret;
    }

    char* strstr(const char* str, const char* substr)
    {
        const char *a = str, *b = substr;
        while (true)
        {
            if (*b == 0) return const_cast<char*>(str);
            if (*a == 0) return nullptr;

            if (*a++ != *b++)
            {
                a = ++str;
                b = substr;
            }
        }
    }

    void strrev(char* str)
    {
        char   a;
        size_t len = strlen(reinterpret_cast<const char*>(str));

        for (size_t i = 0, j = len - 1; i < j; i++, j--)
        {
            a      = str[i];
            str[i] = str[j];
            str[j] = a;
        }
    }
} // extern "C"