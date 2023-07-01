/*
 * Created by vitriol1744 on 23.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "Arch/x86_64/GDT.hpp"
#include "Arch/x86_64/IDT.hpp"

#include "Drivers/Serial.hpp"
#include "Drivers/Terminal.hpp"
#include "Memory/PhysicalMemoryManager.hpp"
#include "Utility/Logger.hpp"

#include <format>
#include <functional>
#include <string>

// Halt and catch fire function.
static void hcf() { halt(); }

// TODO: Thread-safety for Terminal and Logger

#define TryInit(cond, msg, on_success, on_failure)                             \
    {                                                                          \
        Logger::LogChar(RESET_COLOR);                                          \
        if ((cond))                                                            \
        {                                                                      \
            Logger::Log(LogLevel::eTrace,                                      \
                        "Initializing " msg "...\t\t[\u001b[32mok\u001b[0m]"); \
            on_success;                                                        \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            Logger::Log(LogLevel::eError,                                      \
                        "Initializing " msg                                    \
                        "...\t\t[\u001b[31mfailed\u001b[0m]");                 \
            on_failure;                                                        \
        }                                                                      \
    }

extern "C" void kernelStart()
{
#if PH_ARCH == 0x00
    __asm__ volatile("cli");
#endif
    if (Terminal::Initialize()) Logger::EnableTerminalLogging();
    Logger::LogChar(RESET_COLOR);
    Serial::Initialize();
    Logger::EnableSerialLogging();
    TryInit(Serial::Initialize(), "Serial", Logger::EnableSerialLogging(),
            (void)0);
    TryInit(PhysicalMemoryManager::Initialize(), "PMM", (void)0, void(0));

    int                     b        = 34;
    std::function<int(int)> do_stuff = [&](int a) -> int
    {
        b = 12;
        return a + b;
    };
    std::function<int(int)> do_stuff2 = do_stuff;

    int                     value     = do_stuff2(122);
    LogDebug("{}, {}", value, b);
    LogTrace("{}, {}", value, b);
    LogInfo("{}, {}", value, b);
    LogWarn("{}, {}", value, b);
    LogError("{}, {}", value, b);
    LogFatal("{}, {}", value, b);
    LogDebug("{}, {}", value, b);

    std::string s1 = "hello";
    std::string s2 = ", world!";
    std::string s3 = s1 + s2;

    LogInfo("Yo! {} White!", "Mistuh");
    //    Panic("Something happened! {}", "Hello1");

    hcf();
}
