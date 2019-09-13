#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

class ring_buffer final
{
    char* buffer;
    int rows, cols;
    int i, j;
    int next(int index) const;
public:
    ring_buffer();
    ring_buffer(int num_rows, int num_cols);
    ~ring_buffer();
    bool empty() const;
    char* front();
    char* back();
    void push_front();
    void push_back();
};

#endif // __RING_BUFFER_H__
