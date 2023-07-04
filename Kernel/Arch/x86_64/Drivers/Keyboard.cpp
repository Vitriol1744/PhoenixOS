/*
 * Created by vitriol1744 on 03.07.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "Drivers/Keyboard.hpp"

#include "Common.hpp"

#include "Arch/x86_64/Drivers/PIC.hpp"
#include "Arch/x86_64/IDT.hpp"
#include "Arch/x86_64/IO.hpp"
#include "Drivers/Terminal.hpp"

#define I8042_BUFFER   0x60
#define I8042_STATUS   0x64
#define DATA_AVAILABLE 0x01
#define I8042_ACK      0xfa

static char map[0x80]
    = {0,   '\033', '1',  '2', '3',  '4', '5', '6',  '7', '8', '9', '0',
       '-', '=',    '\b', 0,   'q',  'w', 'e', 'r',  't', 'y', 'u', 'i',
       'o', 'p',    '[',  ']', '\n', 0,   'a', 's',  'd', 'f', 'g', 'h',
       'j', 'k',    'l',  ';', '\'', '`', 0,   '\\', 'z', 'x', 'c', 'v',
       'b', 'n',    'm',  ',', '.',  '/', 0,   0,    0,   ' '};

static char shiftMap[0x80]
    = {0,   '\033', '!',  '@', '#',  '$', '%', '^', '&', '*', '(', ')',
       '_', '+',    '\b', 0,   'Q',  'W', 'E', 'R', 'T', 'Y', 'U', 'I',
       'O', 'P',    '{',  '}', '\n', 0,   'A', 'S', 'D', 'F', 'G', 'H',
       'J', 'K',    'L',  ':', '"',  '~', 0,   '|', 'Z', 'X', 'C', 'V',
       'B', 'N',    'M',  '<', '>',  '?', 0,   0,   0,   ' '};

Keyboard::Keyboard()
{
    ClearBuffer();
    SetInterruptVector(0x21);
    Register();
    PIC::UnmaskIRQ(0x01);
}
ssize_t Keyboard::Read(uint8_t* buffer, size_t bytes) { return -1; }
ssize_t Keyboard::Write(const uint8_t* buffer, size_t bytes) { return 0; }

bool    Keyboard::HandleInterrupt(CPUContext* ctx)
{
    while (IO::In<byte>(I8042_STATUS) & DATA_AVAILABLE)
    {
        byte raw     = IO::In<byte>(I8042_BUFFER);
        byte ch      = raw & 0x7f;
        bool pressed = !(raw & 0x80);

// #define KEYBOARD_DEBUG
#ifdef KEYBOARD_DEBUG
        LogDebug("Keyboard::HandleInterrupt: {} {}\n", ch,
                 pressed ? "down" : "up");
#endif
        switch (ch)
        {
            case 0x38:
                if (pressed) modifiers |= eModAlt;
                else modifiers &= ~eModAlt;
            case 0x1d:
                if (pressed) modifiers |= eModControl;
                else modifiers &= ~eModControl;
            case 0x2a:
                if (pressed) modifiers |= eModShift;
                else modifiers &= ~eModShift;
            case I8042_ACK: break;
            default:
                if (modifiers & eModShift) Terminal::PutChar(shiftMap[ch]);
                else Terminal::PutChar(map[ch]);
                //                                key_state_changed(ch,
                //                                pressed);
                break;
        }
    }

    return true;
}
bool Keyboard::OnEndOfInterrupt()
{
    PIC::SendEOI(GetInterruptVector() - 0x20);
    return true;
}

void Keyboard::ClearBuffer()
{
    while (IO::In<byte>(I8042_STATUS) & DATA_AVAILABLE)
        IO::In<byte>(I8042_BUFFER);
}