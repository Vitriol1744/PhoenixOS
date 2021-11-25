#include "stdlib.h"

#include "stdbool.h"
#include "stdint.h"
#include "stddef.h"
#include "string.h"

int atoi(const char* str)
{
    int ret = 0;
    bool is_negative = str[0] == '-';

    int32_t index = 1;
    size_t string_length = strlen(str);

    ret += str[string_length--] - '0';
    for (; string_length > 0 && str[string_length] != '-'; string_length--, index++)
    {
        ret += (str[string_length] - '0') * 10 * index;
    }

    if (is_negative) ret = -ret;
    return ret;
}

char* itoa(int value, char* str, int base)
{
    int i = 0;
    bool is_negative = false;
  
    if (value == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
  
    if (value < 0 && base == 10)
    {
        is_negative = true;
        value = -value;
    }
  
    while (value != 0)
    {
        int rem = value % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        value = value / base;
    }
  
    if (is_negative) str[i++] = '-';
    str[i] = '\0';
  
    int start = 0;
    int end = i -1;
    while (start < end)
    {
        char c = *(str + start);
        *(str + start) = *(str + end);
        *(str + end) = c;
        start++;
        end--;
    }
  
    return str;
}