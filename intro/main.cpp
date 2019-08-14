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
    , time(0)
    {
    }
    int length;
    int time;
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
        while (!groups[k].empty())
        {
            auto& bridge = bridges[k];
            if (groups[k].size() == 1)
            {
                bridge.time += bridge.length / groups[k][0];
                groups[k+1].push_back(groups[k][0]);
                groups[k].pop_back();
            }
            else if (groups[k].size() == 2)
            {
                bridge.time += bridge.length / groups[k][1];
                groups[k+1].push_back(groups[k][0]);
                groups[k+1].push_back(groups[k][1]);
                groups[k].pop_back();
                groups[k].pop_back();
            }
            else if (groups[k].size() == 3)
            {
                bridge.time += bridge.length / groups[k][0];
                bridge.time += bridge.length / groups[k][1];
                bridge.time += bridge.length / groups[k][2];
                groups[k+1].push_back(groups[k][0]);
                groups[k+1].push_back(groups[k][1]);
                groups[k+1].push_back(groups[k][2]);
                groups[k].pop_back();
                groups[k].pop_back();
                groups[k].pop_back();
            }
            else
            {
                for (size_t i = 0; i < groups[k].size()/2; i+=2)
                {
                    size_t j = (groups[k].size() - 1) - i;
                    bridge.time += bridge.length / groups[k][i];
                    bridge.time += bridge.length / groups[k][i+1];
//                    bridge.time += bridge.length / groups[k][j-1];
                    bridge.time += bridge.length / groups[k][j];
                    groups[k+1].push_back(groups[k][j-1]);
                    groups[k+1].push_back(groups[k][j]);
                    groups[k].pop_back();
                    groups[k].pop_back();
                }
            }
            std::sort(groups[k+1].begin(), groups[k+1].end(), std::greater<double>());
            show(groups, bridges);
        }
    }

    return 0;
}

