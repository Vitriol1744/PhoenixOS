#include "KLibC.hpp"

int pow(int num, int power)
{
    int invert  = 0;

    int powMult = 1;

    if ((invert = (power < 0))) power = -power;

    for (int x = 0; x < power; x++) powMult *= num;

    return (invert) ? 1 / powMult : powMult;
}

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

int atoi(const char* str) { atoi(str, strlen(str)); }
int atoi(const char* str, size_t length)
{
    int    integer      = 0;
    bool   isNegative   = str[0] == '-';

    size_t index        = isNegative;
    size_t stringLength = length, power = stringLength - isNegative;

    for (; index < stringLength; index++)
        integer += (str[index] - 48) * pow(10, --power);

    return (isNegative) ? -integer : integer;
}
char* itoa(int32_t value, char* str, int base)
{
    int  i          = 0;
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
        int rem  = value % base;
        str[i++] = (rem > 9) ? (char)((rem - 10) + 'a') : (char)(rem + '0');
        value    = value / base;
    }

    if (isNegative) str[i++] = '-';
    str[i]    = '\0';

    int start = 0;
    int end   = i - 1;
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