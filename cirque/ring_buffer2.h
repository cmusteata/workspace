#ifndef __RING_BUFFER2_H__
#define __RING_BUFFER2_H__

#include <limits>
#include <atomic>

template <typename T>
class ring_buffer final
{
    int length;
    char* data[std::numeric_limits<T>::max()+1];
    std::atomic<T> i, j, next_i, next_j;
public:
    ring_buffer(int len);
    ~ring_buffer();
    bool empty() const;
    bool full() const;
    const char* front() const;
    const char* back() const;
    bool pop_front(char* msg, int msgsize);
    bool push_back(const char* msg, int msgsize);
};

#include "ring_buffer2.hpp"

#endif // __RING_BUFFER2_H__
