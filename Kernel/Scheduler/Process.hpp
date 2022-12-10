#pragma once

#include "Common.hpp"

#include "Utility/Vector.hpp"

#include "Scheduler/Thread.hpp"

using ProcessID = int64_t;

enum class ProcessState
{
    eIdle,
    eRunning,
};

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
    ProcessState state = ProcessState::eIdle;
    uint64_t     timeUsed = 0;

    PageMap      pageMap = {};
};