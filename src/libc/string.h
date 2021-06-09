#pragma once

//TODO: Replace with other value
#define RSIZE_MAX 10000000

#include <stddef.h>
#include "errno.h"

typedef size_t rsize_t;

#define restrict

// String Manipulation

// Copies one string to another
char* strcpy(char* restrict dest, const char* restrict src);
// Copies one string to another
//errno_t strcpy_s(char* restrict dest, rsize_t destsz, const char* restrict src);
// Copies a certain amount of characters from one string to another
char* strncpy(char* restrict dest, const char* restrict src, size_t count);
// Copies a certain amount of characters from one string to another
//errno_t strncpy_s(char* restrict dest, rsize_t destsz, const char* restrict src, rsize_t count);
// Concatenates two strings
char* strcat(char* restrict dest, const char* restrict src);
// Concatenates two strings
//errno_t strcat_s(char* restrict dest, rsize_t destsz, const char* restrict src);
// Concatenates a certain amount of characters of two strings
char* strncat(char* restrict dest, const char* restrict src, size_t count);
// Concatenates a certain amount of characters of two strings
errno_t strncat_s(char* restrict dest, rsize_t destsz, const char* restrict src, rsize_t count);
// transform a string so that strcmp would produce the same result as strcoll
//size_t strxfrm(char* restrict dest, const char* restrict src, size_t count);
// allocates a copy of a string
//char* strdup(const char* str1);
// allocates a copy of a string of specified size
//char* strndup(const char* str1, size_t size);

// String Examination

// Returns the length of a given string
size_t strlen(const char* str);
// Returns the length of a given string
size_t strnlen_s(const char* str, size_t strsz);
// Compares two strings
int strcmp(const char* lhs, const char* rhs);
// Compares a certain amount of characters of two strings
int strncmp(const char* lhs, const char* rhs, size_t count);
// Compares two strings in accordance to the current locale
//int strcoll(const char* lhs, const char* rhs);
//// Finds the first occurrence of a character
//char* strchr(const char* str, int ch);
//// Finds the last occurrence of a character
//char* strrchr(const char* str, int ch);
//// Returns the length of the maximum initial segment that consists
//// of only the characters found in another byte string
//size_t strspn(const char* dest, const char* src);
//// Returns the length of the maximum initial segment that consists
//// of only the characters not found in another byte string
//size_t strcspn(const char* dest, const char* src);
//// Finds the first location of any character in one string, in another string
//char* strpbrk(const char* dest, const char* breakset);
//// Finds the first occurrence of a substring of characters
//char* strstr(const char* str, const char* substr);
//// Finds the next token in a byte string
//char* strtok(char* restrict str, const char* restrict delim);
//// Finds the next token in a byte string
//char* strtok_s(char* restrict str, rsize_t* restrict strmax, const char* restrict delim, char** restrict ptr);
//
//// Character array manipulation
//
//// Searches an array for the first occurrence of a character
//void* memchr(const void* ptr, int ch, size_t count);
//// Compares two buffers
//int memcmp(const void* lhs, const void* rhs, size_t count);
// Fills a buffer with a character
void* memset(void* dest, int ch, size_t count);
// Fills a buffer with a character
errno_t memset_s(void* dest, rsize_t destsz, int ch, rsize_t count);
// Copies one buffer to another
//void* memcpy(void* restrict dest, const void* restrict src, size_t count);
//// Copies one buffer to another
//errno_t memcpy_s(void* restrict dest, rsize_t destsz, const void* restrict src, rsize_t count);
//// Moves one buffer to another
//void* memmove(void* dest, const void* src, size_t count);
//// Moves one buffer to another
//errno_t memmove_s(void* dest, rsize_t destsz, const void* src, rsize_t count);
//// Copies one buffer to another, stopping after the specified delimiter
//void* memccpy(void* restrict dest, const void* restrict src, int c, size_t count);
//
//// 
//
//// Returns a text version of a given error code
//char* strerror(int errnum);
//// Returns a text version of a given error code
//errno_t strerror_s(char* buf, rsize_t bufsz, errno_t errnum);
//// Returns a text version of a given error code
//size_t strerrorlen_s(errno_t errnum);
