/*
 * Created by vitriol1744 on 17.12.2022.
 * Copyright (c) 2022-2022, Szymon Zemke <Vitriol1744@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3
 */
#pragma once

#include "BootInfo.hpp"
#include "Memory/PhysicalMemoryManager.hpp"
#include <new>

template <typename>
class Function;

template <typename Result, typename... Arguments>
class Function<Result(Arguments...)>
{
  public:
    template <typename Functor>
    Function(Functor f)
    {
        functorHolder = new FunctorHolder<Functor, Result, Arguments...>(f);
    }
    ~Function() { delete functorHolder; }

    Result operator()(Arguments... args) const
    {
        return (*functorHolder)(args...);
    }

  private:
    template <typename ReturnType, typename... Args>
    struct FunctorHolderBase
    {
        virtual ~FunctorHolderBase() {}
        virtual ReturnType operator()(Args...)                         = 0;
        virtual void       CopyInto(void*) const                       = 0;
        virtual FunctorHolderBase<Result, Arguments...>* Clone() const = 0;
    };

    template <typename Functor, typename ReturnType, typename... Args>
    struct FunctorHolder final : FunctorHolderBase<Result, Arguments...>
    {
        FunctorHolder(Functor func)
            : f(func)
        {
        }
        ReturnType operator()(Args... args) override { return f(args...); }
        void       CopyInto(void* destination) const override
        {
            new (destination) FunctorHolder(f);
        }
        FunctorHolderBase<Result, Arguments...>* Clone() const override
        {
            return new FunctorHolder(f);
        }

        Functor f;
    };

    FunctorHolderBase<Result, Arguments...>* functorHolder = nullptr;
};