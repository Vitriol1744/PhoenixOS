#ifndef TERMINAL_H
#define TERMINAL_H

#include "common.h"
#include "stivale.h"

extern void        terminalInitialize(stivale2_struct_t* bootloader_data);
extern void        terminalWrite(const char* string, size_t size);
inline static void terminalPutChar(char c) { terminalWrite(&c, 1); }

#endif // TERMINAL_H