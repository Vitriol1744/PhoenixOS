/*
 * Created by vitriol1744 on 26.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "Stacktrace.hpp"

#include "BootInfo.hpp"
#include "Common.hpp"

namespace Stacktrace
{
    namespace
    {
        struct StackFrame
        {
            StackFrame* rbp;
            uintptr_t   rip;
        };

        struct Symbol
        {
            char*     name;
            uintptr_t address;
        };

        size_t    symbolCount                = 0;
        Symbol*   symbols                    = nullptr;
        uintptr_t lowestKernelSymbolAddress  = 0xffffffff;
        uintptr_t highestKernelSymbolAddress = 0x00000000;

        uint64_t  ParseHexDigit(char digit)
        {
            if (digit >= '0' && digit <= '9') return digit - '0';
            Assert(digit >= 'a' && digit <= 'f');

            return (digit - 'a') + 0xa;
        }

        Symbol* GetSymbol(uintptr_t address)
        {
            if (address < lowestKernelSymbolAddress
                || address > highestKernelSymbolAddress)
                return nullptr;
            Symbol* ret = nullptr;

            for (uint32_t i = 0; i < symbolCount; ++i)
            {
                if (!symbols) break;
                if (address < symbols[i + 1].address)
                {
                    ret = &symbols[i];
                    break;
                }
            }

            return ret;
        }
    } // namespace

    void Initialize()
    {
        limine_file* file    = BootInfo::FindModule("ksyms");

        auto*        current = reinterpret_cast<char*>(file->address);
        char*        startOfName;
        for (size_t i = 0; i < 8; ++i)
            symbolCount = (symbolCount << 4) | ParseHexDigit(*(current++));
        current++;

        symbols                      = new Symbol[symbolCount];
        size_t    currentSymbolIndex = 0;

        uintptr_t address            = 0;
        while ((const uint8_t*)current < (uint8_t*)file->address + file->size)
        {
            for (size_t i = 0; i < sizeof(void*) * 2; ++i)
                address = (address << 4) | ParseHexDigit(*(current++));
            current += 3;

            startOfName = current;
            while (*(++current))
                if (*current == '\n') break;
            auto& ksym   = symbols[currentSymbolIndex];
            ksym.address = address;
            ksym.name    = startOfName;

            *current     = '\0';
            if (ksym.address < lowestKernelSymbolAddress)
                lowestKernelSymbolAddress = ksym.address;
            if (ksym.address > highestKernelSymbolAddress)
                highestKernelSymbolAddress = ksym.address;

            ++current;
            ++currentSymbolIndex;
        }

        // TODO: Free pages
    }
    void Print(size_t maxFrames)
    {
        StackFrame* stackFrame;
        // TODO: Stacktrace for aarch64
#if PH_ARCH == PH_ARCH_X86_64
        __asm__ volatile("mov %%rbp, %0" : "=r"(stackFrame) : : "memory");
#else
        return;
#endif
        for (size_t i = 0; stackFrame != nullptr && i < maxFrames; i++)
        {
            uint64_t rip = stackFrame->rip;
            if (rip == 0) break;
            stackFrame     = stackFrame->rbp;
            Symbol* symbol = GetSymbol(rip);

            if (!symbol)
                LogMessage("[\u001b[33mStacktrace\u001b[0m]: {}. ?? <{}>",
                           i + 1, rip);
            else
                LogMessage("[\u001b[33mStacktrace\u001b[0m]: {}. {} <{}>",
                           i + 1, symbol->name, rip);
        }
    }
} // namespace Stacktrace