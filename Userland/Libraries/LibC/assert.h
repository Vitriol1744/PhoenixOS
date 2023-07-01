/*
 * Created by vitriol1744 on 11.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
    __attribute__((noreturn)) extern void __assert_fail(const char*  expr,
                                                        const char*  file,
                                                        unsigned int line,
                                                        const char*  function);
    __attribute__((noreturn)) void        __assert(const char* expr,
                                                   const char* filename, int line);
#ifndef NDEBUG
    #define assert(expr)                                                       \
        (__builtin_expect(!(expr), 0)                                          \
             ? __assert_fail(#expr, __FILE__, __LINE__, __PRETTY_FUNCTION__)   \
             : (void)0)

#else
    #define assert(expr) ((void)(0))
#endif

#ifdef __cplusplus
}
#endif