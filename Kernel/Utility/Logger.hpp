/*
 * Created by vitriol1744 on 24.11.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include <stdarg.h>
#include <stdint.h>

enum class LogLevel
{
    eTrace,
    eInfo,
    eWarn,
    eError,
    eFatal,
};

namespace Logger
{
    void Log(LogLevel level, const char* fmt, ...);
    void Logv(LogLevel level, const char* fmt, va_list& args);

    void EnableE9Logging();
    void EnableTerminalLogging();
    void EnableSerialLogging();
}; // namespace Logger

#define LogTrace(...) Logger::Log(LogLevel::eTrace, __VA_ARGS__)
#define LogInfo(...)  Logger::Log(LogLevel::eInfo, __VA_ARGS__)
#define LogWarn(...)  Logger::Log(LogLevel::eWarn, __VA_ARGS__)
#define LogError(...) Logger::Log(LogLevel::eError, __VA_ARGS__)
#define LogFatal(...) Logger::Log(LogLevel::eFatal, __VA_ARGS__)