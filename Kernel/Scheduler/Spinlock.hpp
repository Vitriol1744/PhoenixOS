/*
 * Created by vitriol1744 on 06.12.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include <stdint.h>

class Spinlock
{
  public:
    Spinlock() = default;

    inline void Lock()
    {
        while (__atomic_test_and_set(&lock, __ATOMIC_ACQUIRE))
        {
            __asm__ __volatile__("nop");
        }
    }
    inline void Unlock() { __atomic_clear(&lock, __ATOMIC_RELEASE); }

  private:
    volatile uint32_t lock = 0;
};