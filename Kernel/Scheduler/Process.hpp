/*
 * Created by vitriol1744 on 02.12.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Common.hpp"

#include "Utility/Vector.hpp"

#include "Scheduler/Thread.hpp"

using ProcessID = int64_t;

enum class ProcessRing
{
    Ring0 = 0,
    Ring3 = 3,
};

struct Process
{
    const char*  processName = nullptr;
    ProcessID    pid = -1;
    ProcessRing  ring = ProcessRing::Ring3;
    Vector<struct Thread*> threads;
    uint64_t     timeUsed = 0;

    PageMap      pageMap = {};
};