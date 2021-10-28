#include "serial.h"

bool serialInitializePort(uint32_t port)
{
    outb(port + 1, 0x00); // Disable all Interrupts
    outb(port + 3, 0x80); // Enable DLAB
    outb(port + 0, 0x03); // Set Divisor to 3 ( Lower  Byte )
    outb(port + 1, 0x00); //                  ( Higher Byte )
    outb(port + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(port + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(port + 4, 0x0B); // IRQs enabled, RTS/DSR set
    outb(port + 4, 0x1E); // Set in loopback mode, test the serial chip

    // Test serial chip(send byte 0xAE and check if serial returns same byte)
    outb(port + 0, 0xAE);
    if (inb(port) != 0xAE)
    {
        //TODO: Some Kind of Logging!
        return false;
    }

    // Disable loopback mode
    outb(port + 4, 0x0F);
    return true;
}

bool serialInitialize(void)
{
    bool ret = false;
    ret |= serialInitializePort(COM1);
    ret |= serialInitializePort(COM2);
    ret |= serialInitializePort(COM3);
    ret |= serialInitializePort(COM4);

    return ret;
}