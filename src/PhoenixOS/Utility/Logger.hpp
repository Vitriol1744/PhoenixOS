#pragma once

#include "Kernel/IO/Terminal.hpp"
#include "Kernel/IO/Serial.hpp"

enum class LogLevel
{
    INFO,
    TRACE,
    WARN,
    ERROR,
    FATAL
};

class Logger
{
    public:
        Logger() = delete;

        inline static void LogIt() { }

        template<typename... Args>
        inline static void LogIt(const char* msg, Args&&... args)
        {
            for (size_t i = 0; i < strlen(msg); i++) Serial::Send(msg[i]);
            LogIt(args...); 
        }

        template<typename... Args>
        inline static void LogIt(long long number, Args&&... args)
        {
            char buffer[20];
            itoa(number, buffer, 10);
            LogIt(buffer, args...);
        }

        template<typename... Args>
        inline static void Log(LogLevel level, const char* text, Args&&... args)
        {
            //TODO: Refactor this later!
            char* foreground_color = const_cast<char*>(ANSI::foreground_white);
            char* background_color = const_cast<char*>(ANSI::background_black);

            switch (level)
            {
                case LogLevel::INFO:
                    foreground_color = const_cast<char*>(ANSI::foreground_cyan);
                    break;
                case LogLevel::TRACE:
                    foreground_color = const_cast<char*>(ANSI::foreground_green);
                    break;
                case LogLevel::WARN:
                    foreground_color = const_cast<char*>(ANSI::foreground_yellow);
                    break;
                case LogLevel::ERROR:
                    foreground_color = const_cast<char*>(ANSI::foreground_red);
                    break;
                case LogLevel::FATAL:
                    foreground_color = const_cast<char*>(ANSI::foreground_white);
                    background_color = const_cast<char*>(ANSI::background_red);
                    break;
            }

            LogIt(foreground_color, background_color, text, args..., "\n");
            foreground_color = const_cast<char*>(ANSI::foreground_white);
            background_color = const_cast<char*>(ANSI::background_black);
            LogIt(foreground_color, background_color);
        }

        template<typename... Args>
        inline static void LogInfo(const char* text, Args&&... args)
        {
            Log(LogLevel::INFO, text, args...);
        }
        
        template<typename... Args>
        inline static void LogTrace(const char* text, Args&&... args)
        {
            Log(LogLevel::TRACE, text, args...);
        }

        template<typename... Args>
        inline static void LogWarn(const char* text, Args&&... args)
        {
            Log(LogLevel::WARN, text, args...);
        }

        template<typename... Args>
        inline static void LogError(const char* text, Args&&... args)
        {
            Log(LogLevel::ERROR, text, args...);
        }

        template<typename... Args>
        inline static void LogFatal(const char* text, Args&&... args)
        {
            Log(LogLevel::FATAL, text, args...);
        }
};

#ifdef DEBUG
    #define LOG_INFO(...) Logger::LogInfo(__VA_ARGS__)
    #define LOG_TRACE(...) Logger::LogTrace(__VA_ARGS__)
    #define LOG_WARN(...) Logger::LogWarn(__VA_ARGS__)
    #define LOG_ERROR(...) Logger::LogError(__VA_ARGS__)
    #define LOG_FATAL(...) Logger::LogFatal(__VA_ARGS__)
#else
    #define LOG_INFO(...)
    #define LOG_TRACE(...)
    #define LOG_WARN(...)
    #define LOG_ERROR(...)
    #define LOG_FATAL(...)
#endif