#include "KLibC.hpp"

extern "C" size_t strlen(const char* str)
{
    size_t length = 0;
    while (*str != '\0')
    {
        ++length;
        ++str;
    }
    return length;
}

extern "C" int strcmp(const char* lhs, const char* rhs)
{
    while (*lhs != '\0')
    {
        if (*lhs != *rhs) return (*lhs - *rhs);
        ++lhs;
        ++rhs;
    }
    return 0;
}
extern "C" int strncmp(const char* lhs, const char* rhs, size_t bytes)
{
    while (*lhs != '\0' && bytes > 0)
    {
        if (*lhs != *rhs) return (*lhs - *rhs);

        ++lhs;
        ++rhs;
        --bytes;
    }
    return 0;
}
extern "C" void* memset(void* dest, char c, size_t bytes)
{
    char* d = reinterpret_cast<char*>(dest);
    while (bytes > 0)
    {
        *d++ = c;
        --bytes;
    }
    return dest;
}
extern "C" void* memcpy(void* dest, const void* src, size_t bytes)
{
    char* d = reinterpret_cast<char*>(dest);
    char* s = reinterpret_cast<char*>(const_cast<void*>(src));

    while (bytes > 0)
    {
        *d++ = *s++;
        --bytes;
    }
    return dest;
}
extern "C" void* memmove(void* dest, const void* src, size_t bytes)
{
    uint8_t*       d = (uint8_t*)dest;
    const uint8_t* s = (uint8_t*)src;

    if (s > d)
    {
        for (size_t i = 0; i < bytes; ++i) d[i] = s[i];
        return dest;
    }

    for (size_t i = bytes; i > 0; --i) d[i - 1] = s[i - 1];

    return dest;
}