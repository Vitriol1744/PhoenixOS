/*
 * Created by vitriol1744 on 11.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "assert.h"

#include "Utility/Logger.hpp"

extern "C"
{
    __attribute__((noreturn)) extern void __assert_fail(const char*  expr,
                                                        const char*  file,
                                                        unsigned int line,
                                                        const char*  function)
    {
#if __STDC_HOSTED__ == 1
        fprintf(stderr, "Assertion Failed(%s::%s:%u): %s", file, function, line,
                expr);
#else
        LogError("Assertion Failed(%s::%s:%u): %s", file, function, line, expr);
#endif
        std::abort();
    }

    __attribute__((noreturn)) void __assert(const char* expr,
                                            const char* filename, int line)
    {
        __assert_fail(expr, filename, line, "");
    }
}