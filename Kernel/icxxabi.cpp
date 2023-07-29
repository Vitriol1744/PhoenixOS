/*
 * Created by vitriol1744 on 13.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include "Common.hpp"

#include <cxxabi.h>

using DestructorFunction = void (*)(void*);

struct AtExitFunctionEntry
{
    DestructorFunction func;
    void*              objptr;
};

constexpr const uint64_t ATEXIT_MAX_FUNCS = 128;

extern "C"
{
    AtExitFunctionEntry __atexit_funcs[ATEXIT_MAX_FUNCS];
    uint64_t            __atexit_func_count = 0;

    void*               __dso_handle        = 0;

    int __cxa_atexit(DestructorFunction func, void* objptr, void* dso)
    {
        if (__atexit_func_count >= ATEXIT_MAX_FUNCS) return -1;
        __atexit_funcs[__atexit_func_count].func   = func;
        __atexit_funcs[__atexit_func_count].objptr = objptr;
        __atexit_func_count++;

        return 0;
    };

    void __cxa_finalize(void* f)
    {
        uint64_t i = __atexit_func_count;
        if (!f)
        {
            while (i--)
            {
                if (__atexit_funcs[i].func)
                    (*__atexit_funcs[i].func)(__atexit_funcs[i].objptr);
            };
            return;
        };

        int64_t findex = -1;
        for (uint64_t j = 0; j < __atexit_func_count; j++)
        {
            if (__atexit_funcs[j].func == f)
            {
                (*__atexit_funcs[j].func)(__atexit_funcs[j].objptr);
                __atexit_funcs[j].func = 0;
                findex                 = j;
            }
        }
        if (findex < 0) return;
        for (uint64_t j = findex; j < __atexit_func_count; j++)
        {
            __atexit_funcs[j].func   = __atexit_funcs[j + 1].func;
            __atexit_funcs[j].objptr = __atexit_funcs[j + 1].objptr;
        }
        --__atexit_func_count;
    }

    void __cxa_pure_virtual() { Panic("__cxa_pure_virtual()"); }

    namespace __cxxabiv1
    {
        int __cxa_guard_acquire(__guard* guard)
        {
            if ((*guard) & 0x0001) return 0;
            if ((*guard) & 0x0100) abort();

            *guard |= 0x0100;
            return 1;
        }

        void __cxa_guard_release(__guard* guard) { *guard |= 0x0001; }

        void __cxa_guard_abort(__guard* guard)
        {
            Panic("__cxa_guard_abort({})", static_cast<void*>(guard));
        }
    } // namespace __cxxabiv1
}