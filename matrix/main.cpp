#include <cassert>
#include <iostream>
#include "matrix.h"

const std::vector<std::vector<int>> before = 
{
    { 1, 2, 3, 4},
    { 5, 6, 7, 8},
    { 9,10,11,12},
    {13,14,15,16}
};

int main(int argc, char**)
{
    assert(argc == 1);
    for (const auto& r : before)
    {
        std::cout << "before:";
        for (auto v : r)
            std::cout << " " << v << ",";
        std::cout << std::endl;
    }

    auto after = matrix_sum(before);
    for (const auto& r : after)
    {
        std::cout << "after:";
        for (auto v : r)
            std::cout << " " << v << ",";
        std::cout << std::endl;
    }

    auto before = matrix_restore(after);
    for (const auto& r : before)
    {
        std::cout << "restore:";
        for (auto v : r)
            std::cout << " " << v << ",";
        std::cout << std::endl;
    }

    return 0;
}

