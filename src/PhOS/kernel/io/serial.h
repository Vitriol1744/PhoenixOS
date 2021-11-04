#ifndef SERIAL_H
#define SERIAL_H

#include "common.h"
#include "kernel/io/io_access.h"

typedef enum serial_port
{
    SERIAL_PORT_COM1 = 0x3F8,
    SERIAL_PORT_COM2 = 0x2F8,
    SERIAL_PORT_COM3 = 0x3E8,
    SERIAL_PORT_COM4 = 0x2E8,
} serial_port_t;

typedef enum
{
    SERIAL_DATA_REGISTER             = 0,
    SERIAL_INTERRUPT_ENABLE_REGISTER = 1,
    SERIAL_DIVISOR_LOW               = 0, // Baud Rate Divisor
    SERIAL_DIVISOR_HIGH              = 1,
    SERIAL_FIFO_CONTROL_REGISTER     = 2,
    SERIAL_INTERRUPT_ID_REGISTER     = 2,
    SERIAL_LINE_CONTROL_REGISTER     = 3,
    SERIAL_MODEM_CONTROL_REGISTER    = 4,
    SERIAL_LINE_STATUS_REGISTER      = 5,
    SERIAL_MODEM_STATUS_REGISTER     = 6,
    SERIAL_SCRATCH_REGISTER          = 7
} serial_register_t;

#define SERIAL_MAKE_REGISTER(port, reg) port + reg

extern bool        serialInitialize(void);

inline static bool serialReceived(serial_port_t port)
{
    return inb(port + 5) & 1;
}
inline static bool serialTransmitEmpty(serial_port_t port)
{
    return inb(port + 5) & 0x20;
}

inline static byte_t serialReadb(serial_port_t port)
{
    while (!serialReceived(port))
        ;
    return inb(port);
}
inline static word_t serialReadw(serial_port_t port)
{
    while (!serialReceived(port))
        ;
    return inw(port);
}
inline static dword_t serialReadl(serial_port_t port)
{
    while (!serialReceived(port))
        ;
    return inl(port);
}

inline static void serialSendb(serial_port_t port, byte_t data)
{
    while (!serialTransmitEmpty(port))
        ;
    outb(port, data);
}
inline static void serialSendw(serial_port_t port, word_t data)
{
    while (!serialTransmitEmpty(port))
        ;
    outw(port, data);
}
inline static void serialSendl(serial_port_t port, dword_t data)
{
    while (!serialTransmitEmpty(port))
        ;
    outl(port, data);
}

#endif // SERIAL_H