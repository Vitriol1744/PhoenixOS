#include "string.h"

// Copies one string to another
char* strcpy(char* restrict dest, const char* restrict src)
{
    char* ptr = dest;
    
    do
    {
        *dest++ = *src;
    }  while(*src++ != 0)
    
    return ptr;
}

//errno_t strcpy_s(char* restrict dest, rsize_t destsz, const char* restrict src)
//{
//    return 0;
//}

// Copies a certain amount of characters from one string to another
char* strncpy(char* restrict dest, const char* restrict src, size_t count)
{
    char* ptr = dest;
    for(size_t i = 0; i < count + 1 || *src != 0; ++i)
    {
        *(dest++) = *(src++);
    }
    return ptr;
}

//errno_t strncpy_s(char* restrict dest, rsize_t destsz, const char* restrict src, rsize_t count)
//{
//    return 0;
//}

// Concatenates two strings
char* strcat(char* restrict dest, const char* restrict src)
{
    char* ptr = dest;
    while(*(dest++) != 0);

    while(*src != 0)
    {
        *(dest++) = *(src++);
    }
    return ptr;
}

//errno_t strncat_s(char* restrict dest, rsize_t destsz, const char* restrict src, rsize_t count)
//{
//    return 0;
//}

// transform a string so that strcmp would produce the same result as strcoll
//size_t strxfrm(char* restrict dest, const char* restrict src, size_t count)
//{
//    return 0;
//}

//char* strdup(const char* str1)
//{
//    
//}

//char* strndup(const char* str1, size_t size)
//{
//    return 0;
//}

size_t strlen(const char* msg)
{
    size_t length = 0;
    while (*msg != 0)
    {
        ++length;
        ++msg;
    }

    return length;
}

size_t strnlen_s(const char* str, size_t strsz)
{
    if(!str) return 0;
    int len = 0;

    for(size_t i = 0; i < strsz && *str++; ++i) ++len;
    
    if(!*str) return strsz;
    return len;
}

int strcmp(const char* lhs, const char* rhs)
{
    while(*lhs != 0 && *rhs != 0)
    {
        if(*lhs > *rhs)
        {
            return 1;
        }
        if(*lhs < *rhs)
        {
            return -1;
        }
        ++rhs;
        ++lhs;
    }

    if(*lhs != 0)
    {
        return 1;
    }
    else if(*rhs != 0)
    {
        return -1;
    }

    return 0;
}

int strncmp(const char* lhs, const char* rhs, size_t count)
{
    while(*lhs != 0 && *rhs != 0 &&  count--)
    {
        if(*lhs > *rhs)
        {
            return 1;
        }
        if(*lhs < *rhs)
        {
            return -1;
        }
        ++rhs;
        ++lhs;
    }
    
    if(*lhs > *rhs)
    {
        return 1;
    }
    else if(*lhs < *rhs)
    {
        return -1;
    }

    return -1;
}

//int strcoll(const char* lhs, const char* rhs)
//{
//    return 0;
//}
//
//char* strchr(const char* str, int ch)
//{
//    return 0;
//}
//
//char* strrchr(const char* str, int ch)
//{
//    return 0;
//}
//
//size_t strspn(const char* dest, const char* src)
//{
//    return 0;
//}
//
//size_t strcspn(const char* dest, const char* src)
//{
//    return 0;
//}
//
//char* strpbrk(const char* dest, const char* breakset)
//{
//    return 0;
//}
//
//char* strstr(const char* str, const char* substr)
//{
//    return 0;
//}
//
//char* strtok(char* restrict str, const char* restrict delim)
//{
//    return 0;
//}
//
//char* strtok_s(char* restrict str, rsize_t* restrict strmax, const char* restrict delim, char** restrict ptr)
//{
//    return 0;
//}
//
//void* memchr(const void* ptr, int ch, size_t count)
//{
//    return 0;
//}
//
//int memcmp(const void* lhs, const void* rhs, size_t count)
//{
//    return 0;
//}

void* memset(void* dest, int ch, size_t count)
{
    unsigned char* ptr = (unsigned char*)dest;
    while (count)
    {
        *ptr = ch;
        count--;
        ptr++;
    }

    return dest;
}

errno_t memset_s(void* dest, rsize_t destsz, int ch, rsize_t count)
{
    //TODO: Replace With Real Error Codes
    if (!dest)
    {
        return -1;
    }
    if (destsz > RSIZE_MAX || count > RSIZE_MAX)
    {
        return -1;
    }
    if (count > destsz)
    {
        return -1;
    }

    memset(dest, ch, count);
    return 0;
}

//void* memcpy(void* restrict dest, const void* restrict src, size_t count)
//{
//    return 0;
//}
//
//errno_t memcpy_s(void* restrict dest, rsize_t destsz, const void* restrict src, rsize_t count)
//{
//    return 0;
//}
//
//void* memmove(void* dest, const void* src, size_t count)
//{
//    return 0;
//}
//
//errno_t memmove_s(void* dest, rsize_t destsz, const void* src, rsize_t count)
//{
//    return 0;
//}
//
//void* memccpy(void* restrict dest, const void* restrict src, int c, size_t count)
//{
//    return 0;
//}
//
//char* strerror(int errnum)
//{
//    return 0;
//}
//
//errno_t strerror_s(char* buf, rsize_t bufsz, errno_t errnum)
//{
//    return 0;
//}
//
//size_t strerrorlen_s(errno_t errnum)
//{
//    return 0;
//}
