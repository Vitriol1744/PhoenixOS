#include "Logger.hpp"

#include "KLibC.hpp"

#include "Drivers/Terminal.hpp"

static bool logE9       = false;
static bool logTerminal = false;
static bool logSerial   = false;

namespace Logger
{
    static void LogChar(char c)
    {
#if PH_ENABLE_LOGGING == false
        return;
#endif
        if (logE9)
            ;
        if (logTerminal) Terminal::PutChar(c);
        if (logSerial)
            ;
    }
    void Log(LogLevel level, const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        Logv(level, fmt, args);
        va_end(args);
    }
    // TODO: Use some fancy templates to make it less ugly, like really... this
    // code is mess
    void Logv(LogLevel level, const char* fmt, va_list& args)
    {
        while (*fmt != '\0')
        {
            if (*fmt == '%')
            {
                fmt++;
                // TODO: We should actually use these flags
                // FIXME: Look at this code again some day and fix all of the
                // problems with it, cause it is really ugly
                bool leftJustify   = false;
                bool plusSign      = false;
                bool spaceIfNoSign = false;
                bool altConversion = false;
                bool zeroPadding   = false;
                while (true)
                {
                    switch (*fmt)
                    {
                        case '\0': return;
                        case '-': leftJustify = true; break;
                        case '+': plusSign = true; break;
                        case ' ': spaceIfNoSign = true; break;
                        case '#': altConversion = true; break;
                        case '0': zeroPadding = true; break;

                        default: goto loop_end;
                    }
                    fmt++;
                }
            loop_end:
                char* numStart  = const_cast<char*>(fmt);
                int   numStrLen = 0;
                while (*fmt != '\0' && *fmt >= '0' && *fmt <= '9')
                {
                    numStrLen++;
                    fmt++;
                }
                int lengthSpecifier = 0;
                if (numStrLen > 0) lengthSpecifier = atoi(numStart, numStrLen);
                if (*fmt == '*')
                {
                    lengthSpecifier = va_arg(args, int);
                    fmt++;
                }
                enum class ArgLength
                {
                    eInt,
                    eLong,
                    eLongLong,
                    eSizeT,
                    ePointer,
                };

                ArgLength argLength = ArgLength::eInt;
                if (*fmt == 'l')
                {
                    fmt++;
                    argLength = ArgLength::eLong;
                    if (*fmt == 'l')
                    {
                        argLength == ArgLength::eLongLong;
                        fmt++;
                    }
                }
                else if (*fmt == 'z')
                {
                    argLength = ArgLength::eSizeT;
                    fmt++;
                }
                bool isSigned = true;
                int  base     = 10;
                switch (*fmt)
                {
                    case 'o':
                        base     = 8;
                        isSigned = false;
                        if (altConversion) LogChar('0');
                        break;
                    case 'p':
                    {
                        isSigned  = false;
                        argLength = ArgLength::ePointer;
                    }
                    case 'X':
                    case 'x':
                        base     = 16;
                        isSigned = false;
                        if (altConversion) Log(level, "0x");
                    case 'u':
                    {
                        switch (argLength)
                        {
                            case ArgLength::eInt:
                            {
                                unsigned int arg = va_arg(args, unsigned int);
                                char         buf[20];
                                char*        str = itoa(arg, buf, base);
                                while (*str) LogChar(*str++);
                            }
                            break;
                            case ArgLength::eLong:
                            {
                                unsigned long arg = va_arg(args, unsigned long);
                                char          buf[20];
                                char*         str = itoa(arg, buf, base);
                                while (*str) LogChar(*str++);
                            }
                            break;
                            case ArgLength::eLongLong:
                            {
                                unsigned long long arg
                                    = va_arg(args, unsigned long long);
                                char  buf[20];
                                char* str = itoa(arg, buf, base);
                                while (*str) LogChar(*str++);
                            }
                            break;
                            case ArgLength::eSizeT:
                            {
                                size_t arg = va_arg(args, size_t);
                                char   buf[20];
                                char*  str = itoa(arg, buf, base);
                                while (*str) LogChar(*str++);
                            }
                            break;
                            case ArgLength::ePointer:
                            {
                                uintptr_t arg = va_arg(args, uintptr_t);
                                char      buf[20];
                                char*     str = itoa(arg, buf, base);
                                while (*str) LogChar(*str++);
                            }
                            default: break;
                        }
                        break;
                    }
                    case 'd':
                    case 'i':
                    {
                        switch (argLength)
                        {
                            case ArgLength::eInt:
                            {
                                int   arg = va_arg(args, int);
                                char  buf[20];
                                char* str = itoa(arg, buf, base);
                                while (*str) LogChar(*str++);
                            }
                            break;
                            case ArgLength::eLong:
                            {
                                long  arg = va_arg(args, long);
                                char  buf[20];
                                char* str = itoa(arg, buf, base);
                                while (*str) LogChar(*str++);
                            }
                            break;
                            case ArgLength::eLongLong:
                            {
                                long long arg = va_arg(args, long long);
                                char      buf[20];
                                char*     str = itoa(arg, buf, base);
                                while (*str) LogChar(*str++);
                            }
                            break;
                            case ArgLength::eSizeT:
                            {
                                size_t arg = va_arg(args, size_t);
                                char   buf[20];
                                char*  str = itoa(arg, buf, base);
                                while (*str) LogChar(*str++);
                            }
                            break;
                            default: break;
                        }
                        break;
                    }
                    case 'c': LogChar(*fmt); break;
                    case 's':
                    {
                        char* str = va_arg(args, char*);
                        while (*str != '\0') LogChar(*str++);
                    }
                    break;
                }
                fmt++;
            }
            else if (*fmt == '\n')
            {
                LogChar('\r');
                LogChar('\n');
                fmt++;
            }
            else LogChar(*fmt++);
        }
    }

    void EnableE9Logging() { logE9 = true; }
    void EnableTerminalLogging() { logTerminal = true; }
    void EnableSerialLogging() { logSerial = true; }
} // namespace Logger