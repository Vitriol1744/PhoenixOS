/*
 * Created by vitriol1744 on 03.07.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include <cstdint>

namespace PIC
{
    void     Remap(uint8_t masterOffset, uint8_t slaveOffset);

    void     MaskIRQ(uint8_t irq);
    void     UnmaskIRQ(uint8_t irq);
    void     MaskAllIRQs();
    void     UnmaskAllIRQs();

    void     SendEOI(uint8_t irq);

    uint16_t GetIRR();
    uint16_t GetISR();
} // namespace PIC