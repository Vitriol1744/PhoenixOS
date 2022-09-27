#include "stdlib.h"

#include "stdbool.h"
#include "stdint.h"
#include "stddef.h"
#include "string.h"
#include "math.h"

int32_t atoi(const char* str)
{
    int32_t integer = 0;
    bool is_negative = str[0] == '-';

    int32_t index = is_negative;
    size_t string_length = strlen(str), power = string_length - is_negative;

    for (; index < string_length; index++)
        integer += (str[index] - 48) * pow(10, --power);


    return (is_negative) ? -integer : integer;
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
