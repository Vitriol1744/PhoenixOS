/*
 * Created by vitriol1744 on 26.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "Drivers/Serial.hpp"

#include "Arch/x86_64/IO.hpp"

namespace Serial
{
    namespace
    {
        enum class Port
        {
            eCom1 = 0x3f8,
            eCom2 = 0x2f8,
            eCom3 = 0x3e8,
            eCom4 = 0x2e8,
        };

        bool InitializePort(Port com)
        {
            word port = static_cast<word>(com);

            // Disable all interrupts
            IO::Out<byte>(port + 1, 0x00);
            // Enable DLAB
            IO::Out<byte>(port + 3, 0x80);
            // Set divisor to 3 (lo byte)
            IO::Out<byte>(port + 0, 0x03);
            //                  (hi byte)
            IO::Out<byte>(port + 1, 0x00);
            // 8 bits, no parity, one stop bit
            IO::Out<byte>(port + 3, 0x03);
            // Enable FIFO, clear them, with 14-byte threshold
            IO::Out<byte>(port + 2, 0xc7);
            // IRQs enabled, RTS/DSR set
            IO::Out<byte>(port + 4, 0x0b);
            // Set in loopback mode, test the serial chip
            IO::Out<byte>(port + 4, 0x1e);
            // Test serial chip (send byte 0xae and check if serial returns same
            // byte)
            IO::Out<byte>(port + 0, 0xae);

            if (IO::In<byte>(port + 0) != 0xae) return false;

            // If serial is not faulty set it in normal operation mode
            // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
            IO::Out<byte>(port + 4, 0x0f);
            return true;
        }
    } // namespace

    bool    Initialize() { return InitializePort(Port::eCom1); }

    uint8_t Read()
    {
        word port = static_cast<word>(Port::eCom1);
        while ((IO::In<byte>(port + 5) & 1) == 0)
            ;

        return IO::In<byte>(port);
    }
    void Write(uint8_t data)
    {
        word port = static_cast<word>(Port::eCom1);
        while ((IO::In<byte>(port + 5) & 0x20) == 0)
            ;

        IO::Out<byte>(port, data);
    }
} // namespace Serial