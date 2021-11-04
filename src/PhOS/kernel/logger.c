#include "logger.h"
#include "kernel/io/serial.h"

#include <stdarg.h>

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
            case 'b': break;
            case 'c':
            {
                char _c = va_arg(args, int);
                logchar(_c);
                break;
            }
            case 'd': break;
            case 'o': break;
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
            case 'x': break;
        }
    }
loop_end:
    va_end(args);
    logchar('\n');
}