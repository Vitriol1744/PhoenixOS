/*
 * Created by vitriol1744 on 02.12.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Arch/x86/CPU.hpp"

using ThreadID = int64_t;

enum class ThreadState
{
    eIdle,
    eRunning,
};

struct Thread
{
    struct Process* process;
    ThreadID    tid;
    ThreadState state;

    uintptr_t   kernelStack;
    uintptr_t   kernelStackSize;

    CPUContext  context;
};