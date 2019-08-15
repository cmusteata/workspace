#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

struct BRIDGE { int length; std::vector<double> paces; };
const std::vector<BRIDGE> yaml = 
{
    { 100, {100, 50, 20, 10}},
    { 250, {2.5}},
    { 150, {25,15}}
};

struct Bridge
{
    Bridge(int len)
    : length(len)
    , time(0.0)
    {
    }
    int length;
    double time;
};

void show(std::vector<std::vector<double>> groups, std::vector<Bridge> bridges)
{
    for (size_t k = 0; k < groups.size(); ++k)
    {
        for (double pace : groups[k])
        {
            std::cout << pace << ",";
        }
        if (k < bridges.size())
            std::cout << " [" << bridges[k].length << " ft, " << bridges[k].time << " min] ";
    }
    std::cout << std::endl;
};

int main()
{
    std::vector<Bridge> bridges;
    std::vector<std::vector<double>> groups;
    for (const auto& y : yaml)
    {
        bridges.emplace_back(y.length);
        groups.emplace_back();
        auto& group = groups.back();
        for (double pace : y.paces)
            group.push_back(pace);
    }
    groups.emplace_back();

    for (size_t k = 0; k < bridges.size(); ++k)
    {
        std::sort(groups[k].begin(), groups[k].end(), std::greater<double>());
        show(groups, bridges);

        auto& bridge = bridges[k];
        for (size_t n = groups[k].size(); n > 0; n = groups[k].size())
        {
            if (n == 1)
            {
                // one-way trip
                // timing for hiker [0]
                bridge.time += bridge.length / groups[k][0];

                // crossing for the last hiker
                groups[k+1].push_back(groups[k][0]);
                groups[k].pop_back();
            }
            else if (n == 2)
            {
                // one-way trip
                // timing for hikers [0] and [1]
                bridge.time += bridge.length / groups[k][1];

                // crossing for the last 2 hikers
                groups[k+1].push_back(groups[k][1]);
                groups[k+1].push_back(groups[k][0]);
                groups[k].pop_back();
                groups[k].pop_back();
            }
            else if (n == 3)
            {
                // first round-trip
                // timing for the fastest pair of hikers [0] and [1] ([0] crosses back)
                bridge.time += bridge.length / groups[k][1];
                bridge.time += bridge.length / groups[k][0];

                // second one-way trip
                // timing for the slowest pair of hikers [2] and [0]
                bridge.time += bridge.length / groups[k][2];

                // transfer for the last 3 hikers
                groups[k+1].push_back(groups[k][2]);
                groups[k+1].push_back(groups[k][1]);
                groups[k+1].push_back(groups[k][0]);
                groups[k].pop_back();
                groups[k].pop_back();
                groups[k].pop_back();
            }
            else
            {
                // first round-trip
                // timing for the fastest pair of hikers [0] and [1] ([0] crosses back)
                bridge.time += bridge.length / groups[k][1];
                bridge.time += bridge.length / groups[k][0];

                // second round-trip
                // timing for the slowest pair of hikers [j] and [j-1] ([1] crosses back)
                size_t j = n - 1;
                bridge.time += bridge.length / groups[k][j];
                bridge.time += bridge.length / groups[k][1];

                // transfer the slowest pair of hikers
                groups[k+1].push_back(groups[k][j-1]);
                groups[k+1].push_back(groups[k][j]);
                groups[k].pop_back();
                groups[k].pop_back();
            }

            // display the progress as we transfer the hikers
            show(groups, bridges);
        }
    }

    return 0;
}

