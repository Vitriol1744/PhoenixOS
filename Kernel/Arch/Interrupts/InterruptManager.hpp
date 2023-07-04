/*
 * Created by vitriol1744 on 03.07.2023.
 * Copyright (c) 2022-2023, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

class InterruptHandler;
namespace InterruptManager
{
    void Initialize();
    void RegisterInterruptHandler(InterruptHandler& interruptHandler);
} // namespace InterruptManager