/*
 * Created by vitriol1744 on 29.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include <optional>
#include <cstdint>

class InterruptHandler
{
    public:
        virtual ~InterruptHandler() = default;

        void SetInterruptNumber(uint8_t interruptNumber) { this->interruptNumber = interruptNumber; }
        uint8_t GetInterruptNumber() const { return interruptNumber.has_value() ? interruptNumber.value() : 0; }

        bool operator()(struct CPUContext* ctx) { return HandleInterrupt(ctx); }
        virtual bool OnEndOfInterrupt() = 0;

    protected:
        InterruptHandler() = default;

        virtual bool HandleInterrupt(CPUContext* ctx) = 0;

    private:
        std::optional<uint8_t> interruptNumber;
};