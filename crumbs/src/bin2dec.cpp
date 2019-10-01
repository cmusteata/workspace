#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

int bin2dec(const char* p)
{
    if (p == nullptr)
        return -1;

    int n = 0;
    if (*p != '\0')
    {
        n = bin2dec(++p);
        n = n << 1;
        n += *p == '0' ? 0 : 1;
    }

    return n;
};

int main(int argc, char* argv[])
{
    assert(argc == 2);
    const char* bin = argv[1];
    std::cout << "bin: " << bin << std::endl;

    int dec = bin2dec(bin);
    std::cout << "dec: " << dec  << std::endl;

    return 0;
}
