#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

int log(unsigned n)
{
    int count = 0; // number of bits to represent the number n
    for (unsigned i = 1; i <= n; i = i << 1)
        count++;
    return count;
};

int log2(unsigned n)
{
    int count = 0;
    for (; n > 0; n = n >> 1)
        count++;
    return count;
};

void show(unsigned n, std::ostringstream& oss)
{
    if (n > 0)
    {
        show(n >> 1, oss);
        oss << n%2;
    }
};

int main(int argc, char* argv[])
{
    assert(argc == 2);
    int n = std::stoi(argv[1]);
    assert(n > 0);

    std::ostringstream oss;
    show(n, oss);
    std::cout << "show(" << n << ") = 0b" << oss.str() << std::endl;

    int logn = log2(n);
    std::cout << "logn = " << logn << std::endl;

    int max = sizeof(n)*8; // max number of bits to represent type int
    int k = max - logn;    // bits to shift to get rid of leftmost bits

    //  n == 5  // 0000 0101
    unsigned c = ~n; // 1111 1010
    c = c << k; // 0100 0000
    c = c >> k; // 0000 0010
    //  c == 2  // ---- ----

    oss.str("");
    show(c, oss);
    std::cout << "compl(" << c << ") = 0b" << oss.str() << std::endl;

    return 0;
}
