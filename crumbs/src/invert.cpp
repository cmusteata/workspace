#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

void invert(int n)
{
    if (n > 0)
    {
        std::ostringstream oss;
        for (int i = 0; i < n; ++i)
            oss << '*';
        std::cout << oss.str() << std::endl;
        invert(--n);
    }
}

int main(int argc, char* argv[])
{
    assert(argc == 2);
    int n = std::stoi(argv[1]);
    invert(n);
    return 0;
}
