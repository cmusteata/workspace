#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

void show(int n, std::ostringstream& oss)
{
    if (n > 0)
    {
        show(n >> 1, oss);
        oss << (n % 2);
    }
};

int main(int argc, char* argv[])
{
    assert(argc == 2);
    int n = std::stoi(argv[1]);

    std::ostringstream oss;
    show(n, oss);
    std::cout << "show(" << n << ") = 0b" << oss.str() << std::endl;

    return 0;
}
