/*
 * Created by vitriol1744 on 03.07.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "Arch/Interrupts/InterruptHandler.hpp"
#include "Utility/UnixTypes.hpp"

class Keyboard : public InterruptHandler
{
  public:
    enum KeyModifier
    {
        eModAlt     = 0x01,
        eModControl = 0x02,
        eModShift   = 0x04,
        eIsPressed  = 0x80,
    };

    Keyboard();

    ssize_t Read(uint8_t* buffer, size_t bytes);
    ssize_t Write(const uint8_t* buffer, size_t bytes);

  private:
    uint8_t      modifiers = 0;

    virtual bool HandleInterrupt(CPUContext* ctx) override;
    virtual bool OnEndOfInterrupt() override;

    void         ClearBuffer();
};