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
    assert(argc == 3);
    int a = std::stoi(argv[1]);
    int b = std::stoi(argv[2]);

    std::ostringstream oss;
    show(a, oss);
    std::cout << "a = " << a << " " << oss.str() << std::endl;

    oss.str("");
    show(b, oss);
    std::cout << "b = " << b << " " << oss.str() << std::endl;

    int anb = a & b; // check a bit

    oss.str("");
    show(anb, oss);
    std::cout << "check bit: a & b = " << anb << " " << oss.str() << std::endl;

    int aob = a | b; // set a bit

    oss.str("");
    show(aob, oss);
    std::cout << "  set bit: a | b = " << aob << " " << oss.str() << std::endl;

    a ^= b; // toggle a bit

    oss.str("");
    show(a, oss);
    std::cout << " flip bit: a ^= b  " << a << " " << oss.str() << std::endl;

    a ^= b; // toggle a bit

    oss.str("");
    show(a, oss);
    std::cout << " flip bit: a ^= b  " << a << " " << oss.str() << std::endl;

    int a_b = a & ~b; // clear a bit

    oss.str("");
    show(a_b, oss);
    std::cout << "clear bit: a &~b = " << a_b << " " << oss.str() << std::endl;

    return 0;
}
