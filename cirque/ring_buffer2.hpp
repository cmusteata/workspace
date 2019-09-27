#ifndef __RING_BUFFER2_HPP__
#define __RING_BUFFER2_HPP__

#include <iostream>
#include <cstring>

template<typename T, size_t size> size_t SizeOf(const T (&)[size]) { return size; };

template <typename T>
ring_buffer<T>::ring_buffer(int len)
: length(len)
, i(0)
, j(1)
, next_i(1)
, next_j(2)
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
    return (next_i > i && next_i >= j) || (next_i < i && next_i <= j);
}

template <typename T>
bool ring_buffer<T>::full() const
{
    return (next_j > j && next_j >= i) || (next_j < j && next_j <= i);
}

template <typename T>
const char* ring_buffer<T>::front() const
{
    return &data[i];
}

template <typename T>
const char* ring_buffer<T>::back() const
{
    return &data[j];
}

template <typename T>
void ring_buffer<T>::pop_front(char* msg, int msgsize)
{
    ++next_i;
    if (empty())
    {
        next_i = i + 1;
        std::cerr << "queue is empty" << std::endl;
        return nullptr;
    }

    char* front = &data[i];
    std::memcpy(msg, front, std::min<int>(length, msgsize));
    ++i;
}

template <typename T>
void ring_buffer<T>::push_back(const char* msg, int msgsize)
{
    ++next_j;
    if (full())
    {
        next_j = j + 1;
        std::cerr << "queue is full" << std::endl;
        return nullptr;
    }

    char* back = &data[j];
    std::memcpy(back, msg, std::min<int>(length, msgsize));
    ++j;
}

#endif // __RING_BUFFER2_HPP__
