#ifndef __RING_BUFFER2_HPP__
#define __RING_BUFFER2_HPP__

#include <iostream>
#include <cstring>

template<typename T, size_t size>
constexpr size_t SizeOf(const T (&)[size])
{
    return size;
};

template <typename T>
ring_buffer<T>::ring_buffer(int len)
: length(len)
, i(0)
, j(1)
{
    size_t size = SizeOf(data) * length;
    char* buffer = static_cast<char*>(operator new(size));
    for (T k = 0; k < SizeOf(data); ++k)
        data[k] = &buffer[k * length];
}

template <typename T>
ring_buffer<T>::~ring_buffer()
{
    char* buffer = data[0];
    operator delete(buffer);
}

template <typename T>
bool ring_buffer<T>::empty() const
{
    return i + 1 == j;
}

template <typename T>
bool ring_buffer<T>::full() const
{
    return j + 1 == i;
}

template <typename T>
const char* ring_buffer<T>::front() const
{
    T next_i = i + 1;
    return next_i == j ? nullptr : &data[next_i];
}

template <typename T>
const char* ring_buffer<T>::back() const
{
    T next_j = j + 1;
    return next_j == i ? nullptr : &data[next_j];
}

template <typename T>
bool ring_buffer<T>::pop_front(char* msg, int msgsize)
{
    if (i + 1 == j) // empty
    {
        std::cerr << "queue is empty" << std::endl;
        return false;
    }

    char* front = &data[++i];
    std::memcpy(msg, front, std::min<int>(length, msgsize));
    return true;
}

template <typename T>
bool ring_buffer<T>::push_back(const char* msg, int msgsize)
{
    if (j + 1 == i)
    {
        std::cerr << "queue is full" << std::endl;
        return false;
    }

    char* back = &data[++j];
    std::memcpy(back, msg, std::min<int>(length, msgsize));
    return true;
}

#endif // __RING_BUFFER2_HPP__
