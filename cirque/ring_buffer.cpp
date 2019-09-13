#include "ring_buffer.h"

ring_buffer::ring_buffer()
: buffer(nullptr)
, rows(1000)
, cols(256)
, i(0)
, j(256)
{    
    buffer = static_cast<char*>(operator new(rows * cols));
}

ring_buffer::ring_buffer(int num_rows, int num_cols)
: buffer(nullptr)
, rows(num_rows)
, cols(num_cols)
, i(0)
, j(num_cols)
{    
    buffer = static_cast<char*>(operator new(rows * cols));
}

ring_buffer::~ring_buffer()
{
    operator delete(buffer);
}

inline 
int ring_buffer::next(int index) const
{
    int n = index + cols;
    return n < rows * cols ? n : 0;
}

inline
bool ring_buffer::empty() const
{
    return next(i) == j;
}

inline
char* ring_buffer::front()
{
    return &buffer[i];
}

inline
char* ring_buffer::back()
{
    return &buffer[j];
}

inline
void ring_buffer::push_front()
{
    ++i;
}

inline
void ring_buffer::push_back()
{
    ++j;
}
