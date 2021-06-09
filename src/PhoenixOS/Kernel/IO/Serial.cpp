#include "Serial.hpp"

#include "Utility/Logger.hpp"

bool Serial::Initialize()
{
    OutB(com1 + 1, 0x00); // Disable all Interrupts
    OutB(com1 + 3, 0x80); // Enable DLAB
    OutB(com1 + 0, 0x03); // Set Divisor to 3 ( Lower  Byte )
    OutB(com1 + 1, 0x00); //                  ( Higher Byte )
    OutB(com1 + 3, 0x03); // 8 bits, no parity, one stop bit
    OutB(com1 + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    OutB(com1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
    OutB(com1 + 4, 0x1E); // Set in loopback mode, test the serial chip
    OutB(com1 + 0, 0xAE); // Test serial chip(send byte 0xAE and check if serial returns same byte)

    if (InB(com1) != 0xAE)
    {
        LOG_ERROR("Failed to Initialize Serial Port!");
        return false;
    }

    OutB(com1 + 4, 0x0F);

    LOG_TRACE("Initialized Serial Port!");
    return true;
}