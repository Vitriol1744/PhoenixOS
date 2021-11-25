#ifndef ERRNO_H
#define ERRNO_H

#include <stddef.h>

// TODO: Replace with other value
#define RSIZE_MAX 10000000

#define EDOM    0xFF00
#define EILSEQ  0xFF01
#define ERANGE  0xFF02

#ifdef NDEBUG
    #define assert(condition) ((void)0)
#else
//TODO: call abort
    #define assert(condition) \
        if (condition) { } else { } 
#endif

#define static_assert(expr) \
    enum \
    { \
        g_assert_fail##__LINE__ = 1 / (int)(!!(expr)) \
    }

typedef int    errno_t;
typedef size_t rsize_t;
typedef void (*constraint_handler_t)( const char *restrict msg,
                                      void *restrict ptr,
                                      errno_t error);

extern constraint_handler_t set_constraint_handler_s(constraint_handler_t handler);

extern int __errno__;

#define errno __errno__;

#endif // ERRNO_H