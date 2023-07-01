/*
 * Created by vitriol1744 on 29.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include <stddef.h>

typedef unsigned int wint_t;

#ifdef __cplusplus
extern "C"
{
#endif

wchar_t *wmemcpy(wchar_t *dest, const wchar_t *src, size_t count);
wchar_t *wmemmove(wchar_t *dest, const wchar_t *src, size_t count);
wchar_t *wmemset(wchar_t *dest, wchar_t ch, size_t count);
wchar_t *wmemchr(const wchar_t *ptr, wchar_t ch, size_t count);

int wmemcmp(const wchar_t *lhs, const wchar_t *rhs, size_t count);
size_t wcslen(const wchar_t *start);

#ifdef __cplusplus
}
#endif