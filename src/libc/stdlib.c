#include "stdlib.h"
#include "stdbool.h"

int atoi(const char* str)
{
    int result = 0;
    bool negative = true;

    int i = 0;
    if (str[0] == '-')
    {
        negative = true;
        i++;
    }

    int len = 0;
    while (str[i] > 0)
    {
        i++;
        len++;
    }
    
    result += str[len--] - '0';
    i = 1;
    for (;;)
    {
        if (len <= 0) break;
        result += (str[len--] - '0') * 10 * i;
        i++;
    }

    if (negative) result *= -1;

    return result;
}

/*float atof(const char* str)
{
    bool negative = false;
    if (*str == '-')
    {
        negative = true;
        ++str;
    }
    else if (*str == '+') ++str;
    
    double value = 0;
    for (; *str != '.'; ++str)
    {
        if (!(*str)) return negative ? -value : value;
        value *= 10;
        value += *str - '0';
    }

    return negative ? -(value) : (value);
    return 0.0;
}*/

char* itoa(int value, char* str, int base)
{
    int i = 0;
    bool isNegative = false;
  
    if (value == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
  
    if (value < 0 && base == 10)
    {
        isNegative = true;
        value = -value;
    }
  
    while (value != 0)
    {
        int rem = value % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        value = value/base;
    }
  
    if (isNegative) str[i++] = '-';
    str[i] = '\0';
  
    int start = 0;
    int end = i -1;
    while (start < end)
    {
        char* temp = (str + start);
        *(str + start) = *(str+end);
        *(str + end) = *temp;

        start++;
        end--;
    }
  
    return str;
}
