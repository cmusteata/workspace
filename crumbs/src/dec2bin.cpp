#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

char* dec2bin(int n, char* p)
{
    if (p == nullptr)
        return p;

    if (n > 0)
    {
        p = dec2bin(n >> 1, p);
        *p = n % 2 == 0 ? '0' : '1';
        p++;
    }

    return p;
};

int main(int argc, char* argv[])
{
    assert(argc == 2);

    int dec = std::stoi(argv[1]);
    std::cout << "dec: " << dec << std::endl;

    char bin[100] = {'\0'};
    dec2bin(dec, bin);
    std::cout << "bin: " << bin << std::endl;

    return 0;
}
