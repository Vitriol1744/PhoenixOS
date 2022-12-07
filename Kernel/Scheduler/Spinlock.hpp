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