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
    template <typename T>
    static void LogNumber(va_list& args, int base)
    {
        char  buf[64];
        char* str = itoa(va_arg(args, T), buf, base);
        while (*str) LogChar(*str++);
    }
    void Log(LogLevel level, const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        Logv(level, fmt, args);
        va_end(args);
    }
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
                int base = 10;
                switch (*fmt)
                {
                    case 'o':
                        base = 8;
                        if (altConversion) LogChar('0');
                    case 'p': argLength = ArgLength::ePointer;
                    case 'X':
                    case 'x':
                        base = 16;
                        if (altConversion) Log(level, "0x");
                    case 'u':
                    {
                        if (argLength == ArgLength::eInt)
                            LogNumber<unsigned int>(args, base);
                        else if (argLength == ArgLength::eLong)
                            LogNumber<unsigned long>(args, base);
                        else if (argLength == ArgLength::eLongLong)
                            LogNumber<unsigned long long>(args, base);
                        else if (argLength == ArgLength::eSizeT)
                            LogNumber<size_t>(args, base);
                        else if (argLength == ArgLength::ePointer)
                            LogNumber<uintptr_t>(args, base);
                        break;
                    }
                    case 'd':
                    case 'i':
                    {
                        if (argLength == ArgLength::eInt)
                            LogNumber<int>(args, base);
                        else if (argLength == ArgLength::eLong)
                            LogNumber<long>(args, base);
                        else if (argLength == ArgLength::eLongLong)
                            LogNumber<long long>(args, base);
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