#include <iostream>
#include "ring_buffer.h"

ring_buffer::ring_buffer()
: buffer(nullptr)
, rows(1000)
, cols(256)
, size(1000*256)
, i(0)
, j(256)
, j2(2*256)
{
    buffer = static_cast<char*>(operator new(size));
}

ring_buffer::ring_buffer(long num_rows, long num_cols)
: buffer(nullptr)
, rows(num_rows)
, cols(num_cols)
, size(num_rows*num_cols)
, i(0)
, j(num_cols)
, j2(2*num_cols)
{
    buffer = static_cast<char*>(operator new(size));
}

ring_buffer::~ring_buffer()
{
    operator delete(buffer);
}

inline 
long ring_buffer::next(long index) const
{
    long n = index + cols;
    return n < size ? n : 0;
}

inline
bool ring_buffer::empty() const
{
    return next(i) == j;
}

inline
bool ring_buffer::full() const
{
    return j2 == i;
}

inline
char* ring_buffer::front()
{
    long next_i = next(i);
    if (next_i == j) // empty
    {
        std::cerr << "queue is empty" << std::endl;
        return nullptr;
    }

    return &buffer[i = next_i];
}

inline
char* ring_buffer::back()
{
    if (j2 == i) // full
    {
        std::cerr << "queue is full" << std::endl;
        return nullptr;
    }

    return &buffer[j];
}
/*
inline
void ring_buffer::push_front()
{
    i = next(i);
}
*/
inline
void ring_buffer::push_back()
{
    j  = j2;
    j2 = next(j);
}

