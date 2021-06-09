#pragma once

#include "Kernel/IO/DirectIO.hpp"

constexpr const int com1 = 0x3F8;

class Serial
{
    public:
        static bool Initialize();

        inline static bool Received() { return InB(com1 + 5) & 1; }
        inline static bool TransmitEmpty() { return InB(com1 + 5) & 0x20; }

        inline static int Read() { while (!Received); return InB(com1); }
        inline static void Send(char data) { while (!TransmitEmpty()); return OutB(com1, data); }
};