#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

void show(int n, std::ostringstream& oss)
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

    std::ostringstream oss;
    show(n, oss);
    std::cout << "n = " << oss.str() << " (dec: " << n << ")"<< std::endl;
    
    int count = -1, m = 0ul;
    for (size_t i = 0; i < sizeof(n)*8; ++i)
    {
        if (n%2 == 0)
        {
            if (count >= 0)
                count++;
        }
        else // == 1
        {
            if (count > m)
                m = count;
            count = 0;
        }
        n = n >> 1;
    }

    oss.str("");
    show(m, oss);
    std::cout << "m = " << m << " (max num of zeroes between two bits set to 1)" << std::endl;

    return 0;
}
