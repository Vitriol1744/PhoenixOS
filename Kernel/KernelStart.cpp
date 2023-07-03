/*
 * Created by vitriol1744 on 23.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "Arch/Arch.hpp"

#include "API/POSIX/sys/stat.h"
#include "Drivers/Serial.hpp"
#include "Drivers/Terminal.hpp"
#include "Memory/PhysicalMemoryManager.hpp"
#include "Utility/Logger.hpp"
#include "Utility/Stacktrace.hpp"
#include "VirtualFileSystem/INode.hpp"
#include "VirtualFileSystem/Initrd/Initrd.hpp"
#include "VirtualFileSystem/VirtualFileSystem.hpp"

#include <mutex>

// Halt and catch fire function.
static void hcf() { halt(); }

// TODO: Thread-safety for Terminal and Logger

#define TryInit(cond, msg, on_success, on_failure)                             \
    {                                                                          \
        Logger::LogChar(RESET_COLOR);                                          \
        if ((cond))                                                            \
        {                                                                      \
            Logger::Log(LogLevel::eTrace,                                      \
                        msg "...\t\t[\u001b[32mok\u001b[0m]");                 \
            on_success;                                                        \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            Logger::Log(LogLevel::eError,                                      \
                        msg "...\t\t[\u001b[31mfailed\u001b[0m]");             \
            on_failure;                                                        \
        }                                                                      \
    }

void IterateDirectories(INode* node, int spaceCount = 0)
{
    char* spaces = new char[spaceCount + 1];
    memset(spaces, ' ', spaceCount);
    spaces[spaceCount] = 0;
    for (auto child : node->GetChildren())
    {
        LogInfo("{}-{}", spaces, child.second->GetName().data());
        if (child.second->mountGate)
        {
            IterateDirectories(child.second->mountGate, spaceCount + 4);
            continue;
        }
        if (child.second->GetType() == INodeType::eDirectory)
            IterateDirectories(child.second, spaceCount + 4);
    }
}

void PrintSegments(std::vector<std::string_view> segments)
{
    LogTrace("Printing segments...");
    for (auto seg : segments) LogInfo("Segment: \"{}\"", seg.data());
}

extern "C" void kernelStart()
{
#if PH_ARCH == PH_ARCH_X86_64
    __asm__ volatile("cli");
#endif
    if (Terminal::Initialize()) Logger::EnableTerminalLogging();
    Logger::LogChar(RESET_COLOR);
    Serial::Initialize();
    Logger::EnableSerialLogging();
    TryInit(Serial::Initialize(), "Initializing Serial",
            Logger::EnableSerialLogging(), (void)0);
    TryInit(PhysicalMemoryManager::Initialize(), "Initializing PMM", (void)0,
            void(0));
    Arch::Initialize();
    Stacktrace::Initialize();

    Assert(VFS::MountRoot("tmpfs"));
    TryInit(Initrd::Initialize(), "Loading initial ramdisk", (void)0, void(0));

    std::string_view somePath      = "./Scheduler/Process.hpp";
    auto             pathSegments1 = Path::SplitPath_(somePath);
    auto             pathSegments2 = Path::SplitPath(somePath);

    PrintSegments(pathSegments1);
    PrintSegments(pathSegments2);
    hcf();

    INode* root = VFS::GetRootINode();
    VFS::CreateNode(root, "/tmpfs", S_IFDIR, INodeType::eDirectory);
    VFS::Mount(root, "", "/tmpfs", "tmpfs");
    VFS::CreateNode(root, "/tmpfs/dir", S_IFDIR, INodeType::eDirectory);
    VFS::CreateNode(root, "/tmpfs/dir/dir", S_IFDIR, INodeType::eDirectory);
    Assert(root != nullptr);
    INode* n = VFS::CreateNode(root, "/VirtualFileSystem/init.cpp", S_IFDIR,
                               INodeType::eDirectory);
    IterateDirectories(n->GetFilesystem()->GetRootINode());

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

    hcf();
}
