#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

extern "C"
{
    size_t strlen(const char* str);
    int    strcmp(const char* lhs, const char* rhs);
    int    strncmp(const char* lhs, const char* rhs, size_t bytes);
    void*  memset(void* dest, char c, size_t bytes);
    void*  memcpy(void* dest, const void* src, size_t bytes);
}

int   atoi(const char* str);
int   atoi(const char* str, size_t length);
char* itoa(int32_t value, char* str, int base);