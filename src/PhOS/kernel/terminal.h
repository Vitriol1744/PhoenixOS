#ifndef TERMINAL_H
#define TERMINAL_H

#include "common.h"
#include "stivale.h"

extern void        terminal_Initialize(stivale2_struct_t* bootloader_data);
extern void        terminal_Write(const char* string, size_t size);
inline static void terminal_PutChar(char c) { terminal_Write(&c, 1); }

#endif // TERMINAL_H