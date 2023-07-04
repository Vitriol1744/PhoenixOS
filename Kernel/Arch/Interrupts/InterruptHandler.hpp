/*
 * Created by vitriol1744 on 29.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Arch/Interrupts/InterruptManager.hpp"

#include <cstdint>
#include <optional>

class InterruptHandler
{
  public:
    virtual ~InterruptHandler() = default;

    void SetInterruptVector(uint8_t interruptNumber)
    {
        interruptVector = interruptNumber;
    }
    uint8_t GetInterruptVector() const
    {
        return interruptVector.has_value() ? interruptVector.value() : 0;
    }

    bool operator()(struct CPUContext* ctx) { return HandleInterrupt(ctx); }
    virtual bool OnEndOfInterrupt() = 0;

  protected:
    InterruptHandler()                            = default;

    virtual bool HandleInterrupt(CPUContext* ctx) = 0;

    inline void  Register()
    {
        InterruptManager::RegisterInterruptHandler(*this);
    }

  private:
    std::optional<uint8_t> interruptVector;
};