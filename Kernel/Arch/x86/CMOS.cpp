/*
 * Created by vitriol1744 on 27.11.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "CMOS.hpp"

#include "Arch/x86/IO.hpp"

namespace CMOS
{
    inline static constexpr const uint8_t CMOS      = 0x70;
    inline static constexpr const uint8_t CMOS_DATA = 0x71;

    void                                  Write(byte reg, byte value)
    {
        IO::Out<byte>(CMOS, reg);
        IO::Wait();

        IO::Out<byte>(CMOS_DATA, value);
    }
    uint8_t Read(byte reg)
    {
        IO::Out<byte>(CMOS, reg);
        IO::Wait();

        return IO::In<byte>(CMOS_DATA);
    }

} // namespace CMOS