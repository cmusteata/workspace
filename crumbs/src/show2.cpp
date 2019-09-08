#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

void show2(int n, std::ostringstream& oss)
{
    for (size_t i = sizeof(n)*8; i > 0; --i)
    {
        size_t k = 1 << (i-1);
        oss << (k & n ? 1 : 0);
    }
};

int main(int argc, char* argv[])
{
    assert(argc == 2);
    int n = std::stoi(argv[1]);

    std::ostringstream oss;
    show2(n, oss);
    std::cout << "show2(" << n << ") = 0b" << oss.str() << std::endl;

    return 0;
}
