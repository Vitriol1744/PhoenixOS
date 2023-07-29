/*
 * Created by vitriol1744 on 16.07.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include <cstdint>

namespace IOAPIC
{
    void SetIRQRedirect(uint32_t lapicID, uint8_t vector, uint8_t irq,
                        bool status);
    void SetGSIRedirect(uint32_t lapicID, uint8_t vector, uint8_t gsi,
                        uint16_t flags, bool status);
} // namespace IOAPIC