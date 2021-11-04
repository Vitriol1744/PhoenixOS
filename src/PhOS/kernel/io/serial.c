#include "serial.h"

bool serialInitializePort(serial_port_t port)
{

    outb(port + SERIAL_INTERRUPT_ENABLE_REGISTER,
         0x00);                                      // Disable all Interrupts
    outb(port + SERIAL_LINE_CONTROL_REGISTER, 0x80); // Enable DLAB
    outb(port + SERIAL_DIVISOR_LOW, 0x03);  // Set Divisor to 3 ( Lower  Byte )
    outb(port + SERIAL_DIVISOR_HIGH, 0x00); //                  ( Higher Byte )
    outb(port + SERIAL_LINE_CONTROL_REGISTER,
         0x03); // 8 bits, no parity, one stop bit
    outb(port + SERIAL_FIFO_CONTROL_REGISTER,
         0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(port + SERIAL_MODEM_CONTROL_REGISTER,
         0x0B); // IRQs enabled, RTS/DSR set
    outb(port + SERIAL_MODEM_CONTROL_REGISTER,
         0x1E); // Set in loopback mode, test the serial chip

    // Test serial chip(send byte 0xAE and check if serial returns same byte)
    outb(port + 0, 0xAE);
    if (inb(port) != 0xAE)
    {
        // TODO: Some Kind of Logging!
        return false;
    }

    // Disable loopback mode
    outb(port + 4, 0x0F);
    return true;
}

bool serialInitialize(void)
{
    bool ret = false;
    ret |= serialInitializePort(SERIAL_PORT_COM1);
    ret |= serialInitializePort(SERIAL_PORT_COM2);
    ret |= serialInitializePort(SERIAL_PORT_COM3);
    ret |= serialInitializePort(SERIAL_PORT_COM4);

    return ret;
}