#ifndef __RING_BUFFER_HPP__
#define __RING_BUFFER_HPP__

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
, i(std::numeric_limits<T>::max())
, j(0)
, next_i(0)
, next_j(1)
{
    size_t rows = SizeOf(data);
    char* buffer = static_cast<char*>(operator new(rows * length));
    for (size_t k = 0; k < rows; ++k)
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
    return next_i == j;
}

template <typename T>
bool ring_buffer<T>::full() const
{
    return next_j == i;
}

template <typename T>
const char* ring_buffer<T>::front() const
{
    return next_i == j ? nullptr : data[next_i];
}

template <typename T>
const char* ring_buffer<T>::back() const
{
    return next_j == i ? nullptr : data[next_j];
}

template <typename T>
bool ring_buffer<T>::pop_front(char* msg, int msgsize)
{
    if (next_i == j) // empty
    {
        std::cerr << "queue is empty" << std::endl;
        return false;
    }

    char* front = data[next_i++];
    std::memcpy(msg, front, std::min<int>(length, msgsize));
    ++i;
    return true;
}

template <typename T>
bool ring_buffer<T>::push_back(const char* msg, int msgsize)
{
    if (next_j == i)
    {
        std::cerr << "queue is full" << std::endl;
        return false;
    }

    char* back = data[next_j++];
    std::memcpy(back, msg, std::min<int>(length, msgsize));
    ++j;
    return true;
}

#endif // __RING_BUFFER_HPP__
