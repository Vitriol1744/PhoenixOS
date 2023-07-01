/*
 * Created by vitriol1744 on 11.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "stdio.h"

#include "Common.hpp"

#include <mutex>

namespace std
{
    [[gnu::noreturn]]
    void terminate() noexcept
    {
        Panic("std::terminate()");
    }
} // namespace std

extern "C"
{
    static std::mutex lock;

    FILE*             stdout = (FILE*)&stdout;
    FILE*             stderr = (FILE*)stderr;

    int               fputc(int c, FILE* stream)
    {
        std::string_view s(reinterpret_cast<const char*>(&c), 1);
        if (stream == stdout) Logger::LogString(s);
        else if (stream == stderr) Logger::Log(LogLevel::eError, s);
        else return -1;

        return 0;
    }
    int fputs(const char* s, FILE* stream)
    {
        if (stream == stdout) Logger::LogString(s);
        else if (stream == stderr) Logger::Log(LogLevel::eError, s);
        else return -1;

        return 0;
    }
    int    fputws(const wchar_t* s, FILE* stream) { return -1; }

    int    fprintf(FILE* stream, const char* format, ...) { return -1; }
    size_t fwrite(const void* buffer, size_t size, size_t count, FILE* stream)
    {
        return -1;
    }

    int printf(const char* format, ...) { return -1; }
    int vprintf(const char* format, va_list args) { return -1; }
    int sprintf(char* s, const char* format, ...) { return -1; }
    int vsprintf(char* s, const char* format, va_list args) { return -1; }
    int snprintf(char* s, size_t count, const char* format, ...) { return -1; }
    int vsnprintf(char* s, size_t count, const char* format, va_list args)
    {
        return -1;
    }
    int vasprintf(char** s, const char* format, va_list args) { return -1; }
    int asprintf(char** s, const char* format, ...) { return -1; }
}
