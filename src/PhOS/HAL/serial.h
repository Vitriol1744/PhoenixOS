#ifndef SERIAL_H
#define SERIAL_H

#include "common.h"
#include "HAL/io_access.h"

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

extern bool serialInitialize(void);

inline static bool serialReceived(word_t port) { return inb(port + 5) & 1; }
inline static bool serialTransmitEmpty(word_t port) { return inb(port + 5) & 0x20; }

inline static byte_t  serialReadb(word_t port) { while (!serialReceived(port)); return inb(port); }
inline static word_t  serialReadw(word_t port) { while (!serialReceived(port)); return inw(port); }
inline static dword_t serialReadl(word_t port) { while (!serialReceived(port)); return inl(port); }

inline static void serialSendb(word_t port, byte_t  data) { while (!serialTransmitEmpty(port)); outb(port, data); }
inline static void serialSendw(word_t port, word_t  data) { while (!serialTransmitEmpty(port)); outw(port, data); }
inline static void serialSendl(word_t port, dword_t data) { while (!serialTransmitEmpty(port)); outl(port, data); }

#endif // SERIAL_H