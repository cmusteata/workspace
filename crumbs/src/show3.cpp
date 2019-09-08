#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

void show(char n, std::ostringstream& oss)
{
    for (size_t i = 8; i > 0; --i)
    {
        char k = 1 << (i-1);
        oss << (k & n ? '1' : '0');
    }
};

int main(int argc, char* argv[])
{
    assert(argc == 2);
    int n = std::stoi(argv[1]);
    char* p = (char*)&n;

    std::cout << "show(" << n << ")" << std::endl;

    std::ostringstream oss;
    show(*(p + 0), oss);
    std::cout << std::hex << (int*)(p + 0) << " 0b" << oss.str() << std::endl;

    oss.str("");
    show(*(p + 1), oss);
    std::cout << std::hex << (int*)(p + 1) << " 0b" << oss.str() << std::endl;

    oss.str("");
    show(*(p + 2), oss);
    std::cout << std::hex << (int*)(p + 2) << " 0b" << oss.str() << std::endl;

    oss.str("");
    show(*(p + 3), oss);
    std::cout << std::hex << (int*)(p + 3) << " 0b" << oss.str() << std::endl;

    return 0;
}
