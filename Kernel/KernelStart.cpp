/*
 * Created by vitriol1744 on 23.06.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#include <stddef.h>
#include "limine.h"

static volatile limine_framebuffer_request framebuffer_request =
{
        .id = LIMINE_FRAMEBUFFER_REQUEST,
        .revision = 0
};

// Halt and catch fire function.
static void hcf()
{
    asm volatile ("cli; hlt");
}

extern "C" void kernelStart()
{
    if (!framebuffer_request.response
        || framebuffer_request.response->framebuffer_count < 1)
        hcf();

    limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];

    for (size_t i = 0; i < 100; i++)
    {
        auto fb_ptr = reinterpret_cast<uint32_t*>(framebuffer->address);
        fb_ptr[i * (framebuffer->pitch / 4) + i] = 0x00ff00;
    }

    hcf();
}