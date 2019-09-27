#include <iostream>
#include "ring_buffer.h"

ring_buffer::ring_buffer()
: buffer(nullptr)
, rows(1000)
, cols(256)
, size(1000*256)
, i(0*256)
, j(1*256)
, next_i(1*256)
, next_j(2*256)
{
    buffer = static_cast<char*>(operator new(size));
}

ring_buffer::ring_buffer(long num_rows, long num_cols)
: buffer(nullptr)
, rows(num_rows)
, cols(num_cols)
, size(num_rows*num_cols)
, i(0*num_cols)
, j(1*num_cols)
, next_i(1*num_cols)
, next_j(2*num_cols)
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
    return next_i == j;
}

inline
bool ring_buffer::full() const
{
    return next_j == i;
}

inline
char* ring_buffer::front()
{
    if (next_i == j) // empty
    {
        std::cerr << "queue is empty" << std::endl;
        return nullptr;
    }

    char* front = &buffer[i];
    i = next_i;
    next_i = next(i);
    return front;
}

inline
char* ring_buffer::back()
{
    if (next_j == i) // full
    {
        std::cerr << "queue is full" << std::endl;
        return nullptr;
    }

    char* back = &buffer[j];
    j = next_j;
    next_j = next(j);
    return back;
}

