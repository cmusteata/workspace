#include <iostream>
#include <vector>
#include <thread>
#include <cassert>
#include <functional>
#include <unistd.h>

std::vector<int> vss;
auto bad = [&](int value) {
    return value > (int)vss.size() / 3;
};

// template <typename F>
// size_t find(F cmp)
// size_t find(bool (*cmp)(size_t))
// size_t find(std::function<bool(size_t)> cmp);

int find(int i, int j)
{
    if (i == j)
        return vss[i];

    int mid = (i+j)/2;
    int mid_i = std::max<int>(i, mid);
    int mid_j = std::min<int>(mid+1, j);
    int curr = vss[mid_i];
    int next = vss[mid_j];

    std::cerr << "["  << vss[i] << "," << curr << "][" << next << "," << vss[j] << "] " << (int)vss.size()/3 << std::endl;

    if (!bad(curr) && bad(next))
        return next;
    if (bad(curr) && bad(next))
        return find(i, mid_i);
    if (!bad(curr) && !bad(next))
        return find(mid_j, j);

    return -1;
};

int main(int argc, char* argv[])
{
    assert(argc == 2);
    int n = std::stoi(argv[1]);
    for (int i = 0; i < n; ++i)
        vss.push_back(i+1);

    int ver = find(0, (int)vss.size() - 1);
    std::cout << "ver: " << ver << std::endl;
    return 0;
}
