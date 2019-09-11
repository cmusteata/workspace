#include "ring_buffer.h"

ring_buffer::ring_buffer()
: buffer(nullptr)
, rows(1000)
, cols(256)
{
    buffer = static_cast<char*>(operator new(rows * cols));
}

ring_buffer::ring_buffer(int num_rows, int num_cols)
: buffer(nullptr)
, rows(num_rows)
, cols(num_cols)
{    
    buffer = static_cast<char*>(operator new(rows * cols));
}

ring_buffer::~ring_buffer()
{
    operator delete(buffer);
}
