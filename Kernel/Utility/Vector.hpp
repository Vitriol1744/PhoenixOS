#pragma once

#include "Common.hpp"

#include "Scheduler/Spinlock.hpp"
#include "Utility/KLibC.hpp"

template <typename T>
class Vector
{
  public:
    Vector() = default;
    Vector(size_t capacity)
        : capacity(capacity)
    {
        Reserve(capacity);
    };
    ~Vector()
    {
        if (data) delete[] data;
    }

    void Reserve(size_t newcapacity)
    {
        if (newcapacity < size) return;

        T* newdata = new T[newcapacity];
        for (int i = 0; i < size; i++) newdata[i] = data[i];

        capacity = newcapacity;
        if (size > 0) delete[] data;
        data = newdata;
    }
    void Resize(size_t newsize)
    {
        Reserve(newsize);
        for (uint32_t i = size; i < capacity; i++) data[i] = {};
        size = newsize;
    }
    void PushBack(T& value)
    {
        if (size == capacity)
        {
            T* newdata = new T[capacity ? 2 * capacity : 1];

            for (int i = 0; i < capacity; i++) { newdata[i] = data[i]; }

            if (size > 0) delete[] data;
            capacity = capacity ? 2 * capacity : 1;
            data     = newdata;
        }

        data[size] = value;
        size++;
    }
    void PopFront()
    {
        uint8_t* ptr = (uint8_t*)&data[0];
        memmove(ptr, (uint8_t*)&data[1], (size - 1) * sizeof(T));

        --size;
    }
    T  PopBack() {}

    T& operator[](size_t index)
    {
        if (index >= size) LogError("Vector: Index out of bounds!");
        return data[index];
    }

    inline size_t GetSize() { return size; }
    inline size_t GetCapacity() { return capacity; }

  private:
    Spinlock lock     = {};
    T*       data     = nullptr;
    size_t   size     = 0;
    size_t   capacity = 0;
};