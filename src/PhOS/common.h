#ifndef COMMON_H
#define COMMON_H 

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define PH_NULL_HANDLE 0
#define PH_ASM(...) __asm__(__VA_ARGS__);
#define PH_ASM_VOL(...) __asm__ volatile(__VA_ARGS__);

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef uint8_t  byte_t;
typedef uint16_t word_t;
typedef uint32_t dword_t;
typedef uint64_t qword_t;

#endif // COMMON_H