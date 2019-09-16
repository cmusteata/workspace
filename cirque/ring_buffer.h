#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

#include <atomic>

class ring_buffer final
{
    char* buffer;
    long rows, cols, size;
    std::atomic<long> i, j, j2;
    long next(long index) const;
public:
    ring_buffer();
    ring_buffer(long num_rows, long num_cols);
    ~ring_buffer();
    bool empty() const;
    bool full() const;
    char* front();
    char* back();
//  void push_front();
    void push_back();
};

#endif // __RING_BUFFER_H__
