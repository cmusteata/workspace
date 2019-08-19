#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <ctime>
#include <cassert>

struct BRIDGE { int length; std::vector<double> paces; };
const std::vector<BRIDGE> yaml = 
{
    { 100, {100, 50, 20, 10}},
    { 250, {2.5}},
    { 150, {25,15}}
};

struct Bridge
{
    int length;
    double time;
 
    Bridge()
    : length(0)
    , time(0.0)
    {
    }

    Bridge(int len)
    : length(len)
    , time(0.0)
    {
    }

    const Bridge& operator +=(const Bridge& bridge)
    {
        length += bridge.length;
        time   += bridge.time;
        return *this;
    }

    friend Bridge operator +(const Bridge& lhs, const Bridge& rhs)
    {
        Bridge bridge;
        bridge.length = lhs.length + rhs.length;
        bridge.time = lhs.time + rhs.time;
        return bridge;
    }

    friend std::ostream& operator <<(std::ostream& os, const Bridge& bridge)
    {
        os << "[" << bridge.length << " ft, " << bridge.time << " min]";
        return os;
    }
};

class Algo
{
    Bridge result;
    std::vector<Bridge> bridges;
    std::vector<std::vector<double>> groups;

    void show(std::vector<std::vector<double>> groups, std::vector<Bridge> bridges)
    {
        for (size_t k = 0; k < groups.size(); ++k)
        {
            for (double pace : groups[k])
                std::cerr << pace << ',';
            if (k < bridges.size())
                std::cerr << bridges[k] << ' ';
        }
        std::cerr << std::endl;
    }

public:
    Algo(const std::vector<BRIDGE>& yaml)
    {
        for (const auto& y : yaml)
        {
            bridges.emplace_back(y.length);
            groups.emplace_back();
            auto& group = groups.back();
            for (double pace : y.paces)
                group.push_back(pace);
        }
        groups.emplace_back();
    }

    void run()
    {
        Bridge total;
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

            // total crossing all bridges
            total += bridge;
        }

        result = total;
    }

    friend std::ostream& operator <<(std::ostream& os, const Algo& algo)
    {
        os << algo.result;
        return os;
    }
};

int main(int argc, char* argv[])
{
    assert(argc == 2);
    std::string filename = argv[1];

    std::cout << "filename: " << filename << std::endl;

    auto begin = std::chrono::system_clock::now();
    time_t begin_time = std::chrono::system_clock::to_time_t(begin);

    Algo algo(yaml);
    algo.run();

    std::cout << "result: " << algo << std::endl;

    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::chrono::duration<double> elapsed_seconds = end - begin;

    std::cout << "begin time: " << std::ctime(&begin_time);
    std::cout << "  end time: " << std::ctime(&end_time);
    std::cout << "   elapsed: " << elapsed_seconds.count() << " seconds"  << std::endl;
    return 0;
}

