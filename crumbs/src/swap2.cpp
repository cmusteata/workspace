#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

void swap(int& a, int& b)
{
    a = a+b;
    b = a-b;
    a = a-b;
};

int main(int argc, char* argv[])
{
    assert(argc == 3);
    int a = std::stoi(argv[1]);
    int b = std::stoi(argv[2]);
    std::cout << "swap [" << a << "," << b << "]" << std::endl;
    swap(a, b);
    std::cout << "done [" << a << "," << b << "]" << std::endl;
    return 0;
}
