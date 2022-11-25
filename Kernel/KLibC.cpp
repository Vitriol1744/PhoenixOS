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
    while (*lhs != '\0' && *lhs == *rhs)
    {
        ++lhs;
        ++rhs;
    }
    if (*lhs == *rhs) return 0;
    return (*lhs - *rhs);
}
extern "C" int strncmp(const char* lhs, const char* rhs, size_t bytes)
{
    while (*lhs != '\0' && *lhs == *rhs && bytes > 0)
    {
        ++lhs;
        ++rhs;
        --bytes;
    }
    if (*lhs == *rhs) return 0;
    return (*lhs - *rhs);
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
