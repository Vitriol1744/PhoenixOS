#include "string.h"

char* strcpy(char* restrict dest, const char* restrict src)
{

}
char* strncpy(char* restrict dest, const char* restrict src, size_t count)
{

}
char* strcat(char* restrict dest, const char* restrict src)
{
    char* ptr = dest;
    while (*dest++ != '\0');

    while (*src != '\0')
    {
        *dest++ = *src++;
    }

    return ptr;
}
char* strncat(char* restrict dest, const char* restrict src, size_t count)
{

}

size_t strlen(const char* str)
{
    size_t length = 0;
    while(*str != '\0')
    {
        ++length;
        ++str;
    }

    return length;
}
size_t strlen_s(const char* str, size_t strsz)
{
    size_t length = 0;
    while (*str != '\0' && strsz > 0)
    {
        ++length;
        --strsz;
        ++str;
    }

    return length;
}
int strcmp(const char* lhs, const char* rhs)
{

}
int strncmp(const char* lhs, const char* rhs, size_t count)
{

}
char* strchr(const char* str, int ch)
{
    while (*str != '\0')
    {
        if (*str == ch) return str;
        ++str;
    }
    
    return NULL;
}

void* memset(void* dest, int ch, size_t count)
{
    while (count)
    {
        *((char*)dest) = ch;
        --count;
    }

    return dest;
}
void* memcpy(void* restrict dest, const void* restrict src, size_t count)
{
    while (count)
    {
        *((char*)dest) = *((char*)src);
        --count;
    }

    return dest;
}
void* memmove(void* dest, const void* src, size_t count)
{
    while (count)
    {
        *((char*)dest) = *((char*)src);
        *((char*)src) = 0;
        --count;
    }

    return dest;
}