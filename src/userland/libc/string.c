#include "string.h"

#include <stdint.h>

#define UNUSED(x) (void)(x)
#define NO_RETURN return 0

extern constraint_handler_t __constraint_handler__;

char* strcpy(char* restrict dest, const char* restrict src)
{
    char* ret = dest;
    do
    {
        *dest++ = *src++;
    } while (*(dest - 1));

    return ret;
}
char* strncpy(char* restrict dest, const char* restrict src, size_t count)
{
    char* ret = dest;
    while (count > 0)
    {
        *dest++ = *src++;
        --count;
    }
    while (count > 0)
    {
        *dest = 0;
        --count;
    }

    return ret;
}
char* strcat(char* restrict dest, const char* restrict src)
{
    char* ret = dest;
    while (*dest) ++dest;

    while (*src != '\0') { *dest++ = *src++; }
    dest = 0;

    return ret;
}
char* strncat(char* restrict dest, const char* restrict src, size_t count)
{
    char* ret = dest;
    while (*dest) ++dest;
    while (*src != 0 && count > 0)
    {
        *dest++ = *src++;
        --count;
    }
    *dest = 0;

    return ret;
}

size_t strlen(const char* str)
{
    char* start = (char*)str;
    while (*str++ != '\0');

    return str - start - 1;
}
size_t strlen_s(const char* str, size_t strsz)
{
    if (*str == 0) return 0;

    char* start = (char*)str;
    while (*str++ != 0) { if (strsz-- <= 0) return strsz; }

    return str - start - 1;
}
int strcmp(const char* lhs, const char* rhs)
{
    while (*lhs != 0 && *rhs != 0 && *rhs == *lhs)
    {
        ++rhs;
        --lhs;
    }
    if (*lhs != *rhs) return *lhs - *rhs;

    return 0;
}
int strncmp(const char* lhs, const char* rhs, size_t count)
{
    while (*lhs != 0 && *rhs != 0 && *rhs == *lhs && count > 0)
    {
        ++rhs;
        ++lhs;
        --count;
    }
    if (*lhs != *rhs) return *lhs - *rhs;

    return 0;
}
char* strchr(const char* str, int ch)
{
    while (*str != (char)ch) str++;

    return (char*)str;
}
extern char* strrchr(const char* str, int ch)
{
    char* ret = NULL;
    while (*(char*)str != 0)
    {
        if (*str == (char)ch) ret = (char*)str;
    }   

    return ret;
}    
size_t strspn(const char* dest, const char* src)
{
    size_t ret = 0;

    while (*(char*)dest != 0)
    {
        for (int i = 0; *(src + i) != 0; i++)
        {
            if (*dest == *(src + i))
            {
                ret++;
                break;
            }
        }
        dest++;
    }

    return ret;
}
size_t strcspn(const char* dest, const char* src)
{
    size_t ret = 0;

    while (*dest != 0)
    {
        for (int i = 0; *(src + i) != 0; i++)
        {
            if (*dest == *(src + i)) return ret;            
        }
        ret++;
        dest++;
    }

    return ret;
}

void* memset(void* dest, int ch, size_t count)
{
    char* ret = dest;

    while (count-- > 0) *(char*)dest = (char)ch;

    return ret;
}
void* memcpy(void* restrict dest, const void* restrict src, size_t count)
{
    char* ret = dest;

    while (count-- > 0)
    {
        *(char*)dest++ = *(char*)src++;
    }

    return ret;
}
void* memmove(void* dest, const void* src, size_t count)
{
    char* ret = dest;

    while (count-- > 0) *(unsigned char*)dest++ = *(unsigned char*)src++;

    return ret;
}
errno_t memmove_s(void* dest, rsize_t destsz, const void* src, rsize_t count)
{
    errno_t error_code = (intptr_t)dest;
    if (dest == NULL)
    {
        __constraint_handler__("memmove_s: dest cannot be nullptr!", NULL, error_code);
        return error_code;
    }
    if (src == NULL)
    {
        __constraint_handler__("memmove_s: src cannot be nullptr!", NULL, error_code);
        return error_code;
    }
    if (count > destsz)
    {
        __constraint_handler__("memmove_s: count cannot be greater than destsz!", NULL, error_code);
        return error_code;
    }

    while (count-- > 0) *(unsigned char*)dest++ = *(unsigned char*)src++;

    return error_code;
}