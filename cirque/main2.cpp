#include <cassert>
#include "ring_buffer2.h"

int main(int argc, char**)
{
    assert(argc == 1);
    ring_buffer<unsigned char> rb(100);

    return 0;
}

