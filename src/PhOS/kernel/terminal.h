#ifndef TERMINAL_H
#define TERMINAL_H

#include "common.h"
#include "stivale.h"

typedef struct
{
    char color[2];
    void(*terminalWrite)(const char* string, size_t size);
} terminal_t;

extern void terminalInitialize(terminal_t* terminal, stivale2_struct_t* bootloader_data);
extern void terminalWrite(terminal_t* terminal, const char* string, size_t size);

#endif // TERMINAL_H