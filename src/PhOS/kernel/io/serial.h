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