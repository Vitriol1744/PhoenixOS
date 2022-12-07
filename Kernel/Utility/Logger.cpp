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
            ; // TODO: outb(0xe9, c);
        if (logTerminal) Terminal::PutChar(c);
        if (logSerial)
            ;
    }
    static void LogString(const char* str)
    {
        while (*str != '\0') LogChar(*str++);
    }
    template <typename T>
    static void LogNumber(va_list& args, int base, bool justifyLeft = false,
                          bool plusSign = false, bool spaceIfNoSign = false,
                          bool zeroPadding = false, size_t lengthSpecifier = 0)
    {
        char   buf[64];
        T      value   = va_arg(args, T);
        char*  str     = itoa(value, buf, base);
        size_t len     = strlen(str);
        char   padding = zeroPadding ? '0' : ' ';
        if (plusSign && lengthSpecifier > 0) lengthSpecifier--;
        if (!justifyLeft)
        {
            while (len < lengthSpecifier)
            {
                LogChar(padding);
                lengthSpecifier--;
            }
        }
        if (value >= 0)
        {
            if (plusSign) LogChar('+');
            else if (spaceIfNoSign) LogChar(' ');
        }
        while (*str) LogChar(*str++);
        if (justifyLeft)
        {
            while (len < lengthSpecifier)
            {
                LogChar(padding);
                lengthSpecifier--;
            }
        }
    }
    void Log(LogLevel level, const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        Logv(level, fmt, args);
        va_end(args);
        Terminal::PrintString("\n\r");
    }
    void Logv(LogLevel level, const char* fmt, va_list& args)
    {
        switch (level)
        {
            case LogLevel::eTrace:
                Terminal::SetForegroundColor(0x00ff00);
                Terminal::SetBackgroundColor(0x000000);

                break;
            case LogLevel::eInfo:
                Terminal::SetForegroundColor(0x00ffff);
                Terminal::SetBackgroundColor(0x000000);
                break;
            case LogLevel::eWarn:
                Terminal::SetForegroundColor(0xffff00);
                Terminal::SetBackgroundColor(0x000000);
                break;
            case LogLevel::eError:
                Terminal::SetForegroundColor(0xff0000);
                Terminal::SetBackgroundColor(0x000000);
                break;
            case LogLevel::eFatal:
                Terminal::SetForegroundColor(0xffffff);
                Terminal::SetBackgroundColor(0xff0000);
                break;
        }

        while (*fmt != '\0')
        {
            if (*fmt == '%')
            {
                fmt++;
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
                if (numStrLen > 0)
                    lengthSpecifier = atoi<size_t>(numStart, numStrLen);
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
                        argLength = ArgLength::eLongLong;
                        fmt++;
                    }
                }
                else if (*fmt == 'z')
                {
                    argLength = ArgLength::eSizeT;
                    fmt++;
                }
                int base = 10;
#define LogNum(type)                                                           \
    LogNumber<type>(args, base, leftJustify, plusSign, spaceIfNoSign,          \
                    zeroPadding, lengthSpecifier)
                switch (*fmt)
                {
                    case 'o':
                        base = 8;
                        if (altConversion) LogChar('0');
                    case 'p': argLength = ArgLength::ePointer;
                    case 'X':
                    case 'x':
                        base = 16;
                        if (altConversion)
                        {
                            LogChar('0');
                            LogChar('x');
                        }
                    case 'u':
                    {
                        if (argLength == ArgLength::eInt) LogNum(unsigned int);
                        else if (argLength == ArgLength::eLong)
                            LogNum(unsigned long);
                        else if (argLength == ArgLength::eLongLong)
                            LogNum(unsigned long);
                        else if (argLength == ArgLength::eSizeT) LogNum(size_t);
                        else if (argLength == ArgLength::ePointer)
                            LogNum(uintptr_t);
                        break;
                    }
                    case 'd':
                    case 'i':
                    {
                        if (argLength == ArgLength::eInt) LogNum(int);
                        else if (argLength == ArgLength::eLong) LogNum(long);
                        else if (argLength == ArgLength::eLongLong)
                            LogNum(long);
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

        Terminal::SetForegroundColor(0xffffff);
        Terminal::SetBackgroundColor(0x000000);
    }

    void EnableE9Logging() { logE9 = true; }
    void EnableTerminalLogging() { logTerminal = true; }
    void EnableSerialLogging() { logSerial = true; }
} // namespace Logger