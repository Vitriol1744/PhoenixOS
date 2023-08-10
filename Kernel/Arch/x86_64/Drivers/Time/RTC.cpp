/*
 * Created by vitriol1744 on 10.08.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "RTC.hpp"

#include "Arch/x86_64/CMOS.hpp"

namespace RTC
{
    inline static constexpr u8 BcdToBin(u8 value)
    {
        return (value >> 4) * 10 + (value & 15);
    }

    u8 GetCentury()
    {
        // TODO(V1tri0l1744): Read century from FADT
        return BcdToBin(CMOS::Read(CMOS_RTC_CENTURY));
    }
    u8   GetYear() { return BcdToBin(CMOS::Read(CMOS_RTC_YEAR)); }
    u8   GetMonth() { return BcdToBin(CMOS::Read(CMOS_RTC_MONTH)); }
    u8   GetDay() { return BcdToBin(CMOS::Read(CMOS_RTC_MONTH_DAY)); }
    u8   GetHour() { return BcdToBin(CMOS::Read(CMOS_RTC_HOURS)); }
    u8   GetMinute() { return BcdToBin(CMOS::Read(CMOS_RTC_MINUTES)); }
    u8   GetSecond() { return BcdToBin(CMOS::Read(CMOS_RTC_SECONDS)); }

    u8   GetTime() { return GetHour() * 3600 + GetMinute() * 60 + GetSecond(); }

    void Sleep(u64 seconds)
    {
        u64 lastSecond = GetTime();
        while (lastSecond == GetTime()) __asm__ volatile("pause");

        lastSecond = GetTime() + seconds;
        while (lastSecond != GetTime()) __asm__ volatile("pause");
    }
} // namespace RTC