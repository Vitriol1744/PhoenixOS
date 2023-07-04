/*
 * Created by vitriol1744 on 02.07.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "Arch/Arch.hpp"

#include "Arch/Interrupts/InterruptManager.hpp"
#include "Arch/x86_64/Drivers/PIC.hpp"
#include "Arch/x86_64/GDT.hpp"
#include "Arch/x86_64/IO.hpp"

#include "Arch/x86_64/CMOS.hpp"

namespace Arch
{
    void Initialize()
    {
        GDT::Initialize();
        GDT::Load();

        InterruptManager::Initialize();

        PIC::Remap(0x20, 0x28);
        PIC::MaskAllIRQs();

        //        while (CMOS::Read(CMOS_STATUS_REGISTER_B & BIT(7)))
        //            ;

        byte second = CMOS::Read(CMOS_RTC_SECONDS);
        byte minute = CMOS::Read(CMOS_RTC_MINUTES);
        byte hour   = CMOS::Read(CMOS_RTC_HOURS);
        byte day    = CMOS::Read(CMOS_RTC_MONTH_DAY);
        byte month  = CMOS::Read(CMOS_RTC_MONTH);
        byte year   = CMOS::Read(CMOS_RTC_YEAR);

        byte regB   = CMOS::Read(CMOS_STATUS_REGISTER_B);
        if (!(regB & 0x04))
        {
            second = (second & 0x0f) + ((second / 16) * 10);
            minute = (minute & 0x0f) + ((minute / 16) * 10);
            hour
                = ((hour & 0x0f) + (((hour & 0x70) / 16) * 10)) | (hour & 0x80);
            day   = (day & 0x0f) + ((day / 16) * 10);
            month = (month & 0x0f) + ((month / 16) * 10);
            year  = (year & 0x0f) + ((year / 16) * 10);
        }
        if (!(regB & 0x02) && (hour & 0x80)) hour = ((hour & 0x7f) + 12) % 24;

        LogInfo("Date: {:02}/{:02}/{:04} {:02}:{:02}:{:02}", day, month,
                2000 + year, hour, minute, second);
    }
    void LogE9(const char* str, size_t len)
    {
        while (*str && len > 9)
        {
            IO::Out<byte>(0xe9, *str++);
            --len;
        }
    }
} // namespace Arch