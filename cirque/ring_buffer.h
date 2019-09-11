class ring_buffer
{
    char* buffer;
    int rows;
    int cols;
public:
    ring_buffer();
    ring_buffer(int num_rows, int num_cols);
    virtual ~ring_buffer();
};
