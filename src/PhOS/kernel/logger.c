#include "logger.h"
#include "kernel/io/serial.h"

#include <stdarg.h>
#include <stdlib.h>

#include "kernel/terminal.h"

void logchar(char c) { serialSendb(SERIAL_PORT_COM1, c); }

void log(log_level_t log_level, const char* fmt, ...)
{
    char* c;
    PH_UNUSED(log_level);

    va_list args;
    va_start(args, fmt);

    for (c = (char*)fmt; *c != '\0'; c++)
    {
        while (*c != '%')
        {
            if (*c == 0) goto loop_end;
            logchar(*c);
            c++;
        }

        c++;
        switch (*c)
        {
            case 'b':
            {
                uint64_t value = va_arg(args, uint64_t);
                char string[20];
                char* str = string;
                itoa(value, str, 2);
                while (*str != 0)
                {
                    logchar(*str);
                    ++str;
                }
                break;
            }
            case 'c':
            {
                char _c = va_arg(args, int);
                logchar(_c);
                break;
            }
            case 'd':
            {
                uint64_t value = va_arg(args, uint64_t);
                char string[20];
                char* str = string;
                itoa(value, str, 10);
                while (*str != 0)
                {
                    logchar(*str);
                    ++str;
                }
                break;
            }
            case 'i':
            {
                int64_t value = va_arg(args, int64_t);
                char string[20];
                char* str = string;
                itoa(value, str, 10);
                while (*str != 0)
                {
                    logchar(*str);
                    ++str;
                }
                break;
            }
            case 'o':
            {
                uint64_t value = va_arg(args, uint64_t);
                char string[20];
                char* str = string;
                itoa(value, str, 8);
                while (*str != 0)
                {
                    logchar(*str);
                    ++str;
                }
                break;
            }
            case 'r':
            {
                char* string = va_arg(args, char*);
                char* start = string;
                while (*string != '\0') ++string;
                while (string >= start) logchar(*string--);
                break;
            }
            case 's':
            {
                char* string = va_arg(args, char*);
                while (*string != '\0')
                {
                    logchar(*string);
                    ++string;
                }
                break;
            }
            case 'u':
            {
                uint64_t value = va_arg(args, uint64_t);
                char string[20];
                char* str = string;
                itoa(value, str, 10);
                while (*str != 0)
                {
                    logchar(*str);
                    ++str;
                }
                break;
            }
            case 'x':
            {
                uint64_t value = va_arg(args, uint64_t);
                char string[20];
                char* str = string;
                itoa(value, str, 16);
                while (*str != 0)
                {
                    logchar(*str);
                    ++str;
                }
                break;
            }
        }
    }
loop_end:
    va_end(args);
    logchar('\n');
}