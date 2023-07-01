/*
 * Created by vitriol1744 on 28.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "Logger.hpp"

#include "Arch/Arch.hpp"
#include "Drivers/Serial.hpp"
#include "Drivers/Terminal.hpp"

namespace Logger
{
    namespace
    {
        bool logE9       = false;
        bool logSerial   = false;
        bool logTerminal = false;
    } // namespace

    void EnableE9Logging() { logE9 = true; }
    void EnableSerialLogging() { logSerial = true; }
    void EnableTerminalLogging() { logTerminal = true; }

    void LogChar(uint64_t c)
    {
        size_t len = 1;
        if (c == RESET_COLOR) len = 4;
        else if (c > 255) len = 5;
        LogString(reinterpret_cast<const char*>(&c), len);
        if (c == '\n') LogChar('\r');
    }

    void LogString(std::string_view string)
    {
        LogString(string, string.length());
    }
    void LogString(std::string_view str, size_t len)
    {
        if (logE9) Arch::LogE9(str.data(), len);
        if (logSerial) Serial::LogString(str.data(), len);
        if (logTerminal) Terminal::PrintString(str.data(), len);
    }
} // namespace Logger