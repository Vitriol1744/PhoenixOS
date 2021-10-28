#ifndef STDLIB_H
#define STDLIB_H

#ifdef __cplusplus
extern "C" 
{
#endif

// Converts string literal to int
int atoi(const char* str);
// Converts string literal to double
//float atof(const char* str);

char* itoa(int value, char* str, int base);

#ifdef __cplusplus
}
#endif

#endif // STDLIB_H