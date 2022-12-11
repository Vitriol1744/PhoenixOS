/*
 * Created by vitriol1744 on 30.11.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Scheduler/Process.hpp"
#include "Scheduler/Thread.hpp"

namespace Scheduler
{
    void                           Initialize();

    __attribute__((noreturn)) void Await();
    __attribute__((noreturn)) void Yield();

    Process*                      CreateProcess(PageMap& page_map);
    Thread*  CreateKernelThread(Process* process, uintptr_t rip);
    Thread*  CreateUserThread(Process* process, uintptr_t rip);

    Process* GetKernelProcess();
} // namespace Scheduler
